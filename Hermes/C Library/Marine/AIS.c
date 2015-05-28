/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../h/Marine/AIS.h"
#include "../h/Utilities/Utilities.h"
#include "../h/SessionParameters.h"
#include <funcapi.h>
#include <access/htup_details.h>
#include <utils/builtins.h>

/*
 * bits.c
 * gps.h
 * driver_ais.c
 * drivers.c
 */

#define BITS_PER_BYTE 8
#define NMEA_MAX 91
#define AIS_SHIPNAME_MAXLEN 20
#define AIS_NR_FIELDS 7

int32 computeAISChecksum(const char *ais) {
	const char *c = NULL;
	size_t i;

	int32 sum;
	size_t ais_len = strlen(ais);

	sum = 0;
	for (c = ais + 1, i = 1; *c != '*' && i < ais_len; c++, i++)
		sum ^= *c;

	return sum;
}

PGDLLEXPORT Datum computeAISChecksumV1(PG_FUNCTION_ARGS) {
	int32 ret;
	char *ais = text_to_cstring(PG_GETARG_TEXT_P(0));

	ret = computeAISChecksum(ais);
	pfree(ais);

	PG_RETURN_INT32(ret);
}
PG_FUNCTION_INFO_V1(computeAISChecksumV1);

bool isAuxiliaryMMSI(int32 mmsi) {
	return (mmsi / 10000000 == 98) ? true : false;
}

PGDLLEXPORT Datum isAuxiliaryMMSIV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(isAuxiliaryMMSI(PG_GETARG_INT32(0)));
}
PG_FUNCTION_INFO_V1(isAuxiliaryMMSIV1);

static int fields(const char *ais, char *field[]) {
	char *cp = NULL;
	size_t ais_len;

	int nfields;
	char *ais_copy = pstrdup(ais);

	field[0] = ais_copy;
	nfields = 1;

	ais_len = strlen(ais_copy);
	for (cp = ais_copy; cp < ais_copy + ais_len; cp++) {
		if (*cp == ',') {
			if (nfields < AIS_NR_FIELDS) {
				*cp = '\0';
				field[nfields] = cp + 1;
			}

			nfields++;
		}
	}

	return nfields;
}

static size_t bits(const char *data_payload, char buf[], size_t buf_size) {
	int i, j;
	char ch;

	size_t bits_len = 0;
	size_t data_payload_len = strlen(data_payload);

	memset(buf, '\0', buf_size);

	for (i = 0; i < data_payload_len; i++) {
		ch = ((ch = data_payload[i] - 48) > 40) ? ch - 8 : ch;

		for (j = 5; j >= 0; j--) {
			if ((ch >> j) & 0x01) {
				buf[bits_len / 8] |= (1 << (7 - bits_len % 8));
			}
			bits_len++;
		}
	}

	return bits_len;
}

static uint64 ubits(char buf[], unsigned int start, unsigned int width) {
	uint64 fld = 0;
	unsigned int i;
	unsigned end;

	for (i = start / BITS_PER_BYTE; i < (start + width + BITS_PER_BYTE - 1) / BITS_PER_BYTE; i++) {
		fld <<= BITS_PER_BYTE;
		fld |= (unsigned char) buf[i];
	}

	end = (start + width) % BITS_PER_BYTE;
	if (end != 0) {
		fld >>= (BITS_PER_BYTE - end);
	}

	fld &= ~(-1LL << width);

	return fld;
}

static int64 sbits(char buf[], unsigned int start, unsigned int width) {
	uint64 fld = ubits(buf, start, width);

	if (fld & (1LL << (width - 1))) {
		fld |= (-1LL << (width - 1));
	}

	return (int64) fld;
}

static char *uchars(char bitvec[], unsigned int start, int count, bool xml) {
	const char sixchr[64] = "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_ !\"#$%&'()*+,-./0123456789:;<=>?";

	int i, j;
	char newchar, *buf = NULL, *ret = NULL;

	buf = (char *) palloc(((xml ? count * 5 : count) + 1) * sizeof(char));

	/* six-bit to ASCII */
	for (i = 0, j = 0; i < count; i++, j++) {
		newchar = sixchr[ubits(bitvec, start + 6 * i, 6U)];

		if (newchar == '@')
			break;

		if (xml && newchar == '&') {
			buf[j] = '&';
			j++;
			buf[j] = 'a';
			j++;
			buf[j] = 'm';
			j++;
			buf[j] = 'p';
			j++;
			buf[j] = ';';
		} else if (xml && newchar == '<') {
			buf[j] = '&';
			j++;
			buf[j] = 'l';
			j++;
			buf[j] = 't';
			j++;
			buf[j] = ';';
		} else if (xml && newchar == '>') {
			buf[j] = '&';
			j++;
			buf[j] = 'g';
			j++;
			buf[j] = 't';
			j++;
			buf[j] = ';';
		} else
			buf[j] = newchar;
	}
	buf[j] = '\0';

	/* trim spaces on right end */
	for (i = j - 1; i >= 0; i--) {
		if (buf[i] == ' ' || buf[i] == '@')
			buf[i] = '\0';
		else
			break;
	}

	ret = (char *) palloc((i + 2) * sizeof(char));
	memcpy(ret, buf, (i + 2) * sizeof(char));

	pfree(buf);

	return ret;
}

PGDLLEXPORT Datum decodeAISXML(PG_FUNCTION_ARGS) {
	char *ais = text_to_cstring(PG_GETARG_TEXT_P(0));

	char *field[AIS_NR_FIELDS];
	char bitvec[2048];
	size_t bits_len;

	int32 type; // Message Type
	int32 repeat; // Repeat Indicator
	int32 mmsi; // MMSI
	int32 partno; // Part Number (for Message Type 24)

	char *result_s = NULL;
	xmltype *result = NULL;

	int16 error = 0;
	char error_dsc[81];

	if (fields(ais, field) == AIS_NR_FIELDS) {
		if (strcmp(field[0], "!AIVDM") == 0 || strcmp(field[0], "!AIVDO") == 0) {
			if (strcmp(field[1], "1") == 0 && strcmp(field[2], "1") == 0) {
				bits_len = bits(field[5], bitvec, sizeof(bitvec));

				type = (int32) ubits(bitvec, 0, 6);
				repeat = (int32) ubits(bitvec, 6, 2);
				mmsi = (int32) ubits(bitvec, 8, 30);

				if (type == 1 || type == 2 || type == 3) {
					if (bits_len != 168) {
						error = 4;
						snprintf(error_dsc, 81, "%d.	%s", error, "bits_len != 168");
					}

					asprintfHermes(&result_s, "<AIS type=\"%d\" repeat=\"%d\" mmsi=\"%d\" error=\"%s\">""\n"
						"\t<status>"HE_FS_UINT64"</status>""\n"
						"\t<turn>"HE_FS_INT64"</turn>""\n"
						"\t<speed>"HE_FS_UINT64"</speed>""\n"
						"\t<accuracy>%s</accuracy>""\n"
						"\t<lon>%f</lon>""\n"
						"\t<lat>%f</lat>""\n"
						"\t<course>"HE_FS_UINT64"</course>""\n"
						"\t<heading>"HE_FS_UINT64"</heading>""\n"
						"\t<second>"HE_FS_UINT64"</second>""\n"
						"\t<maneuver>"HE_FS_UINT64"</maneuver>""\n"
						"\t<raim>%s</raim>""\n"
						"\t<radio>"HE_FS_UINT64"</radio>""\n"
						"</AIS>", type, repeat, mmsi, (error == 4) ? error_dsc : "", ubits(bitvec, 38, 4), sbits(bitvec, 42, 8), ubits(bitvec, 50, 10), ubits(bitvec, 60, 1) != 0 ? "true" : "false", sbits(bitvec, 61, 28) / 600000.0, sbits(bitvec, 89, 27) / 600000.0, ubits(bitvec, 116, 12), ubits(bitvec, 128, 9), ubits(bitvec, 137, 6), ubits(bitvec, 143, 2), (ubits(bitvec, 148, 1) != 0) != 0 ? "true" : "false", ubits(bitvec, 149, 20));
				} else if (type == 4 || type == 11) {
					if (bits_len != 168) {
						error = 5;
						snprintf(error_dsc, 81, "%d.	%s", error, "bits_len != 168");
					}

					asprintfHermes(&result_s, "<AIS type=\"%d\" repeat=\"%d\" mmsi=\"%d\" error=\"%s\">""\n"
						"\t<year>"HE_FS_UINT64"</year>""\n"
						"\t<month>"HE_FS_UINT64"</month>""\n"
						"\t<day>"HE_FS_UINT64"</day>""\n"
						"\t<hour>"HE_FS_UINT64"</hour>""\n"
						"\t<minute>"HE_FS_UINT64"</minute>""\n"
						"\t<second>"HE_FS_UINT64"</second>""\n"
						"\t<accuracy>%s</accuracy>""\n"
						"\t<lon>%.6f</lon>""\n"
						"\t<lat>%.6f</lat>""\n"
						"\t<epfd>"HE_FS_UINT64"</epfd>""\n"
						"\t<raim>%s</raim>""\n"
						"\t<radio>"HE_FS_UINT64"</radio>""\n"
						"</AIS>", type, repeat, mmsi, (error == 5) ? error_dsc : "", ubits(bitvec, 38, 14), ubits(bitvec, 52, 4), ubits(bitvec, 56, 5), ubits(bitvec, 61, 5), ubits(bitvec, 66, 6), ubits(bitvec, 72, 6), ubits(bitvec, 78, 1) != 0 ? "true" : "false", sbits(bitvec, 79, 28) / 600000.0, sbits(bitvec, 107, 27) / 600000.0, ubits(bitvec, 134, 4), ubits(bitvec, 148, 1) != 0 ? "true" : "false", ubits(bitvec, 149, 19));
				} else if (type == 5) {
					if (bits_len != 424) {
						error = 6;
						snprintf(error_dsc, 81, "%d.	%s", error, "bits_len != 424");
					}

					asprintfHermes(&result_s, "<AIS type=\"%d\" repeat=\"%d\" mmsi=\"%d\" error=\"%s\">""\n"
						"\t<ais_version>"HE_FS_UINT64"</ais_version>""\n"
						"\t<imo>"HE_FS_UINT64"</imo>""\n"
						"\t<callsign>%s</callsign>""\n"
						"\t<shipname>%s</shipname>""\n"
						"\t<shiptype>"HE_FS_UINT64"</shiptype>""\n"
						"\t<to_bow>"HE_FS_UINT64"</to_bow>""\n"
						"\t<to_stern>"HE_FS_UINT64"</to_stern>""\n"
						"\t<to_port>"HE_FS_UINT64"</to_port>""\n"
						"\t<to_starboard>"HE_FS_UINT64"</to_starboard>""\n"
						"\t<epfd>"HE_FS_UINT64"</epfd>""\n"
						"\t<month>"HE_FS_UINT64"</month>""\n"
						"\t<day>"HE_FS_UINT64"</day>""\n"
						"\t<hour>"HE_FS_UINT64"</hour>""\n"
						"\t<minute>"HE_FS_UINT64"</minute>""\n"
						"\t<draught>%f</draught>""\n"
						"\t<destination>%s</destination>""\n"
						"\t<dte>"HE_FS_UINT64"</dte>""\n"
						"</AIS>", type, repeat, mmsi, (error == 6) ? error_dsc : "", ubits(bitvec, 38, 2), ubits(bitvec, 40, 30), uchars(bitvec, 70, 7, true), uchars(bitvec, 112, AIS_SHIPNAME_MAXLEN, true), ubits(bitvec, 232, 8), ubits(bitvec, 240, 9), ubits(bitvec, 249, 9), ubits(bitvec, 258, 6), ubits(bitvec, 264, 6), ubits(bitvec, 270, 4), ubits(bitvec, 274, 4), ubits(bitvec, 278, 5), ubits(bitvec, 283, 5), ubits(bitvec, 288, 6), ubits(bitvec, 294, 8) / 10.0, uchars(bitvec, 302, 20, true), ubits(bitvec, 422, 1));
				} else if (type == 9) {
					if (bits_len != 168) {
						error = 7;
						snprintf(error_dsc, 81, "%d.	%s", error, "bits_len != 168");
					}

					asprintfHermes(&result_s, "<AIS type=\"%d\" repeat=\"%d\" mmsi=\"%d\" error=\"%s\">""\n"
						"\t<alt>"HE_FS_UINT64"</alt>""\n"
						"\t<speed>"HE_FS_UINT64"</speed>""\n"
						"\t<accuracy>%s</accuracy>""\n"
						"\t<lon>%.6f</lon>""\n"
						"\t<lat>%.6f</lat>""\n"
						"\t<course>"HE_FS_UINT64"</course>""\n"
						"\t<second>"HE_FS_UINT64"</second>""\n"
						"\t<regional>"HE_FS_UINT64"</regional>""\n"
						"\t<dte>"HE_FS_UINT64"</dte>""\n"
						"\t<assigned>%s</assigned>""\n"
						"\t<raim>%s</raim>""\n"
						"\t<radio>"HE_FS_UINT64"</radio>""\n"
						"</AIS>", type, repeat, mmsi, (error == 7) ? error_dsc : "", ubits(bitvec, 38, 12), ubits(bitvec, 50, 10), ubits(bitvec, 60, 1) != 0 ? "true" : "false", sbits(bitvec, 61, 28) / 600000.0, sbits(bitvec, 89, 27) / 600000.0, ubits(bitvec, 116, 12), ubits(bitvec, 128, 6), ubits(bitvec, 134, 8), ubits(bitvec, 142, 1), ubits(bitvec, 146, 1) != 0 ? "true" : "false", ubits(bitvec, 147, 1) != 0 ? "true" : "false", ubits(bitvec, 148, 19));
				} else if (type == 14) {
					if (bits_len < 40 || bits_len > 1008) {
						error = 8;
						snprintf(error_dsc, 81, "%d.	%s", error, "AIVDM message size is out of range 40-1008");
					}

					asprintfHermes(&result_s, "<AIS type=\"%d\" repeat=\"%d\" mmsi=\"%d\" error=\"%s\">""\n"
						"\t<text>%s</text>""\n"
						"</AIS>", type, repeat, mmsi, (error == 8) ? error_dsc : "", uchars(bitvec, 40, (int) (bits_len - 40) / 6, true));
				} else if (type == 18) {
					if (bits_len != 168) {
						error = 9;
						snprintf(error_dsc, 81, "%d.	%s", error, "bits_len != 168");
					}

					asprintfHermes(&result_s, "<AIS type=\"%d\" repeat=\"%d\" mmsi=\"%d\" error=\"%s\">""\n"
						"\t<reserved>"HE_FS_UINT64"</reserved>""\n"
						"\t<speed>"HE_FS_UINT64"</speed>""\n"
						"\t<accuracy>%s</accuracy>""\n"
						"\t<lon>%.6f</lon>""\n"
						"\t<lat>%.6f</lat>""\n"
						"\t<course>"HE_FS_UINT64"</course>""\n"
						"\t<heading>"HE_FS_UINT64"</heading>""\n"
						"\t<second>"HE_FS_UINT64"</second>""\n"
						"\t<regional>"HE_FS_UINT64"</regional>""\n"
						"\t<cs>%s</cs>""\n"
						"\t<display>%s</display>""\n"
						"\t<dsc>%s</dsc>""\n"
						"\t<band>%s</band>""\n"
						"\t<msg22>%s</msg22>""\n"
						"\t<assigned>%s</assigned>""\n"
						"\t<raim>%s</raim>""\n"
						"\t<radio>"HE_FS_UINT64"</radio>""\n"
						"</AIS>", type, repeat, mmsi, (error == 9) ? error_dsc : "", ubits(bitvec, 38, 8), ubits(bitvec, 46, 10), ubits(bitvec, 56, 1) != 0 ? "true" : "false", sbits(bitvec, 57, 28) / 600000.0, sbits(bitvec, 85, 27) / 600000.0, ubits(bitvec, 112, 12), ubits(bitvec, 124, 9), ubits(bitvec, 133, 6), ubits(bitvec, 139, 2), ubits(bitvec, 141, 1) != 0 ? "true" : "false", ubits(bitvec, 142, 1) != 0 ? "true" : "false", ubits(bitvec, 143, 1) != 0 ? "true" : "false", ubits(bitvec, 144, 1) != 0 ? "true" : "false", ubits(bitvec, 145, 1) != 0 ? "true" : "false", ubits(bitvec, 146, 1) != 0 ? "true" : "false", ubits(bitvec, 147, 1) != 0 ? "true" : "false", ubits(bitvec, 148, 20));
				} else if (type == 19) {
					if (bits_len != 312) {
						error = 10;
						snprintf(error_dsc, 81, "%d.	%s", error, "bits_len != 312");
					}

					asprintfHermes(&result_s, "<AIS type=\"%d\" repeat=\"%d\" mmsi=\"%d\" error=\"%s\">""\n"
						"\t<reserved>"HE_FS_UINT64"</reserved>""\n"
						"\t<speed>"HE_FS_UINT64"</speed>""\n"
						"\t<accuracy>%s</accuracy>""\n"
						"\t<lon>%.6f</lon>""\n"
						"\t<lat>%.6f</lat>""\n"
						"\t<course>"HE_FS_UINT64"</course>""\n"
						"\t<heading>"HE_FS_UINT64"</heading>""\n"
						"\t<second>"HE_FS_UINT64"</second>""\n"
						"\t<regional>"HE_FS_UINT64"</regional>""\n"
						"\t<shipname>%s</shipname>""\n"
						"\t<shiptype>"HE_FS_UINT64"</shiptype>""\n"
						"\t<to_bow>"HE_FS_UINT64"</to_bow>""\n"
						"\t<to_stern>"HE_FS_UINT64"</to_stern>""\n"
						"\t<to_port>"HE_FS_UINT64"</to_port>""\n"
						"\t<to_starboard>"HE_FS_UINT64"</to_starboard>""\n"
						"\t<epfd>"HE_FS_UINT64"</epfd>""\n"
						"\t<raim>%s</raim>""\n"
						"\t<dte>"HE_FS_UINT64"</dte>""\n"
						"\t<assigned>%s</assigned>""\n"
						"</AIS>", type, repeat, mmsi, (error == 10) ? error_dsc : "", ubits(bitvec, 38, 8), ubits(bitvec, 46, 10), ubits(bitvec, 56, 1) != 0 ? "true" : "false", sbits(bitvec, 57, 28) / 600000.0, sbits(bitvec, 85, 27) / 600000.0, ubits(bitvec, 112, 12), ubits(bitvec, 124, 9), ubits(bitvec, 133, 6), ubits(bitvec, 139, 4), uchars(bitvec, 143, 20, true), ubits(bitvec, 263, 8), ubits(bitvec, 271, 9), ubits(bitvec, 280, 9), ubits(bitvec, 289, 6), ubits(bitvec, 295, 6), ubits(bitvec, 299, 4), ubits(bitvec, 302, 1) != 0 ? "true" : "false", ubits(bitvec, 305, 1), ubits(bitvec, 306, 1) != 0 ? "true" : "false");
				} else if (type == 24) {
					partno = (int32) ubits(bitvec, 38, 2);

					switch (partno) {
					case 0:
						if (bits_len != 160) {
							error = 11;
							snprintf(error_dsc, 81, "%d.	%s", error, "bits_len != 160");
						}

						asprintfHermes(&result_s, "<AIS type=\"%d\" partno=\"%d\" repeat=\"%d\" mmsi=\"%d\" error=\"%s\">""\n"
							"\t<shipname>%s</shipname>""\n"
							"</AIS>", type, partno, repeat, mmsi, (error == 11) ? error_dsc : "", uchars(bitvec, 40, AIS_SHIPNAME_MAXLEN, true));

						break;
					case 1:
						if (bits_len != 168) {
							error = 12;
							snprintf(error_dsc, 81, "%d.	%s", error, "bits_len != 168");
						}

						if (isAuxiliaryMMSI(mmsi)) {
							asprintfHermes(&result_s, "<AIS type=\"%d\" partno=\"%d\" repeat=\"%d\" mmsi=\"%d\" error=\"%s\">""\n"
								"\t<shiptype>"HE_FS_UINT64"</shiptype>""\n"
								"\t<vendorid>%s</vendorid>""\n"
								"\t<callsign>%s</callsign>""\n"
								"\t<mothership_mmsi>"HE_FS_UINT64"</mothership_mmsi>""\n"
								"</AIS>", type, partno, repeat, mmsi, (error == 12) ? error_dsc : "", ubits(bitvec, 40, 8), uchars(bitvec, 48, 7, true), uchars(bitvec, 90, 7, true), ubits(bitvec, 132, 30));
						} else {
							asprintfHermes(&result_s, "<AIS type=\"%d\" partno=\"%d\" repeat=\"%d\" mmsi=\"%d\" error=\"%s\">""\n"
								"\t<shiptype>"HE_FS_UINT64"</shiptype>""\n"
								"\t<vendorid>%s</vendorid>""\n"
								"\t<callsign>%s</callsign>""\n"
								"\t<to_bow>"HE_FS_UINT64"</to_bow>""\n"
								"\t<to_stern>"HE_FS_UINT64"</to_stern>""\n"
								"\t<to_port>"HE_FS_UINT64"</to_port>""\n"
								"\t<to_starboard>"HE_FS_UINT64"</to_starboard>""\n"
								"</AIS>", type, partno, repeat, mmsi, (error == 12) ? error_dsc : "", ubits(bitvec, 40, 8), uchars(bitvec, 48, 7, true), uchars(bitvec, 90, 7, true), ubits(bitvec, 132, 9), ubits(bitvec, 141, 9), ubits(bitvec, 150, 6), ubits(bitvec, 156, 6));
						}

						break;
					default:
						asprintfHermes(&result_s, "<AIS type=\"%d\" partno=\"%d\" repeat=\"%d\" mmsi=\"%d\" error=\"%s\">""\n"
							"</AIS>", type, partno, repeat, mmsi, "13.	AIVDM message type 24 of subtype unknown");

						break;
					}
				} else {
					asprintfHermes(&result_s, "<AIS type=\"%d\" repeat=\"%d\" mmsi=\"%d\" error=\"%s\">""\n"
						"</AIS>", type, repeat, mmsi, "14.	unsupported type");
				}
			} else {
				asprintfHermes(&result_s, "<AIS error=\"%s\">""\n"
					"</AIS>", "3.	Currently fragments are not supported");
			}
		} else {
			asprintfHermes(&result_s, "<AIS error=\"%s\">""\n"
				"</AIS>", "2.	Not AIVDM/AIVDO sentence");
		}
	} else {
		asprintfHermes(&result_s, "<AIS error=\"%s%d\">""\n"
			"</AIS>", "1.	nfields != ", AIS_NR_FIELDS);
	}

	result = DatumGetXmlP(DirectFunctionCall1(xml_in, CStringGetDatum(result_s)));

	pfree(result_s);
	pfree(field[0]);
	pfree(ais);

	PG_RETURN_XML_P(result);
}
PG_FUNCTION_INFO_V1(decodeAISXML);

PGDLLEXPORT Datum decodeAIS(PG_FUNCTION_ARGS) {
	char *ais = text_to_cstring(PG_GETARG_TEXT_P(0));

	char *field[AIS_NR_FIELDS];
	char bitvec[2048];

	int32 type;
	int32 mmsi;

	bool nulls[2] = {true, true};

	TupleDesc resultTupleDesc;
	Datum resultValues[2];
	HeapTuple result;

	if (fields(ais, field) == AIS_NR_FIELDS) {
		if (strcmp(field[0], "!AIVDM") == 0 || strcmp(field[0], "!AIVDO") == 0) {
			if (strcmp(field[1], "1") == 0 && strcmp(field[2], "1") == 0) {
				bits(field[5], bitvec, sizeof(bitvec));

				type = (int32) ubits(bitvec, 0, 6);
				mmsi = (int32) ubits(bitvec, 8, 30);

				resultValues[0] = Int32GetDatum(type);
				resultValues[1] = Int32GetDatum(mmsi);

				nulls[0] = false;
				nulls[1] = false;
			}
		}
	}

	pfree(field[0]);
	pfree(ais);

	if (get_call_result_type(fcinfo, NULL, &resultTupleDesc) != TYPEFUNC_COMPOSITE)
		ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("function returning record called in context that cannot accept type record")));

	BlessTupleDesc(resultTupleDesc);
	result = heap_form_tuple(resultTupleDesc, resultValues, nulls);

	PG_RETURN_DATUM(HeapTupleGetDatum(result));
}
PG_FUNCTION_INFO_V1(decodeAIS);

PGDLLEXPORT Datum decodeAISPositionReport(PG_FUNCTION_ARGS) {
	char *ais = text_to_cstring(PG_GETARG_TEXT_P(0));

	char *field[AIS_NR_FIELDS];
	char bitvec[2048];

	int32 type;
	int32 mmsi;

	float8 lon;
	float8 lat;

	int32 heading;
	int64 turn;

	uint64 speed;
	uint64 course;

	bool nulls[9] = {true, true, true, true, true, true, true, true, true};

	TupleDesc resultTupleDesc;
	Datum resultValues[9];
	HeapTuple result;

	if (fields(ais, field) == AIS_NR_FIELDS) {
		if (strcmp(field[0], "!AIVDM") == 0 || strcmp(field[0], "!AIVDO") == 0) {
			if (strcmp(field[1], "1") == 0 && strcmp(field[2], "1") == 0) {
				bits(field[5], bitvec, sizeof(bitvec));

				type = (int32) ubits(bitvec, 0, 6);
				mmsi = (int32) ubits(bitvec, 8, 30);

				resultValues[0] = Int32GetDatum(type);
				resultValues[1] = Int32GetDatum(mmsi);

				nulls[0] = false;
				nulls[1] = false;

				if (type == 1 || type == 2 || type == 3 || type == 18 || type == 19) {
					if (type == 1 || type == 2 || type == 3) {
						resultValues[2] = Int32GetDatum((int32) ubits(bitvec, 38, 4));
						nulls[2] = false;
					}

					if (type == 1 || type == 2 || type == 3)
						lon = sbits(bitvec, 61, 28) / 600000.0;
					else
						lon = sbits(bitvec, 57, 28) / 600000.0;
					
					if (-180.0 <= lon && lon <= 180.0) {
						resultValues[3] = Float8GetDatum(lon);
						nulls[3] = false;
					}

					if (type == 1 || type == 2 || type == 3)
						lat = sbits(bitvec, 89, 27) / 600000.0;
					else
						lat = sbits(bitvec, 85, 27) / 600000.0;

					if (-90.0 <= lat && lat <= 90.0) {
						resultValues[4] = Float8GetDatum(lat);
						nulls[4] = false;
					}

					if (type == 1 || type == 2 || type == 3)
						heading = (int32) ubits(bitvec, 128, 9);
					else
						heading = (int32) ubits(bitvec, 124, 9);

					if (0 <= heading && heading < 360) {
						resultValues[5] = Int32GetDatum(heading);
						nulls[5] = false;
					}

					if (type == 1 || type == 2 || type == 3) {
						turn = sbits(bitvec, 42, 8);
						if (turn == 0) {
							resultValues[6] = Float8GetDatum(0.0);
							nulls[6] = false;
						} else if (0 < turn && turn <= 126) {
							resultValues[6] = Float8GetDatum(pow(turn / 4.733, 2.0));
							nulls[6] = false;
						} else if (-126 <= turn && turn < 0) {
							resultValues[6] = Float8GetDatum(pow(turn / 4.733, 2.0) * -1.0);
							nulls[6] = false;
						}
					}

					if (type == 1 || type == 2 || type == 3)
						speed = ubits(bitvec, 50, 10);
					else
						speed = ubits(bitvec, 46, 10);

					if (speed <= 1022) {
						resultValues[7] = Float8GetDatum(speed / 10.0);
						nulls[7] = false;
					}

					if (type == 1 || type == 2 || type == 3)
						course = ubits(bitvec, 116, 12);
					else
						course = ubits(bitvec, 112, 12);

					if (course < 3600) {
						resultValues[8] = Float8GetDatum(course / 10.0);
						nulls[8] = false;
					}
				}
			}
		}
	}

	pfree(field[0]);
	pfree(ais);

	if (get_call_result_type(fcinfo, NULL, &resultTupleDesc) != TYPEFUNC_COMPOSITE)
		ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("function returning record called in context that cannot accept type record")));

	BlessTupleDesc(resultTupleDesc);
	result = heap_form_tuple(resultTupleDesc, resultValues, nulls);

	PG_RETURN_DATUM(HeapTupleGetDatum(result));
}
PG_FUNCTION_INFO_V1(decodeAISPositionReport);

PGDLLEXPORT Datum decodeAISStaticData(PG_FUNCTION_ARGS) {
	char *ais = text_to_cstring(PG_GETARG_TEXT_P(0));

	char *field[AIS_NR_FIELDS];
	char bitvec[2048];

	int32 type;
	int32 mmsi;

	int32 i;
	char *tmp = NULL;

	bool nulls[16] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};

	TupleDesc resultTupleDesc;
	Datum resultValues[16];
	HeapTuple result;

	if (fields(ais, field) == AIS_NR_FIELDS) {
		if (strcmp(field[0], "!AIVDM") == 0 || strcmp(field[0], "!AIVDO") == 0) {
			if (strcmp(field[1], "1") == 0 && strcmp(field[2], "1") == 0) {
				bits(field[5], bitvec, sizeof(bitvec));

				type = (int32) ubits(bitvec, 0, 6);
				mmsi = (int32) ubits(bitvec, 8, 30);

				resultValues[0] = Int32GetDatum(type);
				resultValues[1] = Int32GetDatum(mmsi);

				nulls[0] = false;
				nulls[1] = false;
				
				if (type == 5 || type == 19) {
					if (type == 5) {
						resultValues[2] = Int32GetDatum((int32) ubits(bitvec, 40, 30));
						nulls[2] = false;
					}

					if (type == 5) {
						tmp = uchars(bitvec, 70, 7, false);
						resultValues[3] = PointerGetDatum(cstring_to_text(tmp));
						pfree(tmp);

						nulls[3] = false;
					}

					if (type == 5)
						tmp = uchars(bitvec, 112, AIS_SHIPNAME_MAXLEN, false);
					else
						tmp = uchars(bitvec, 143, AIS_SHIPNAME_MAXLEN, false);

					resultValues[4] = PointerGetDatum(cstring_to_text(tmp));
					pfree(tmp);

					nulls[4] = false;

					if (type == 5) {
						resultValues[5] = Int32GetDatum((int32) ubits(bitvec, 240, 9));
						resultValues[6] = Int32GetDatum((int32) ubits(bitvec, 249, 9));
						resultValues[7] = Int32GetDatum((int32) ubits(bitvec, 258, 6));
						resultValues[8] = Int32GetDatum((int32) ubits(bitvec, 264, 6));
					} else {
						resultValues[5] = Int32GetDatum((int32) ubits(bitvec, 271, 9));
						resultValues[6] = Int32GetDatum((int32) ubits(bitvec, 280, 9));
						resultValues[7] = Int32GetDatum((int32) ubits(bitvec, 289, 6));
						resultValues[8] = Int32GetDatum((int32) ubits(bitvec, 295, 6));
					}

					nulls[5] = false;
					nulls[6] = false;
					nulls[7] = false;
					nulls[8] = false;

					if (type == 5)
						resultValues[9] = Int32GetDatum((int32) ubits(bitvec, 232, 8));
					else
						resultValues[9] = Int32GetDatum((int32) ubits(bitvec, 263, 8));

					nulls[9] = false;

					if (type == 5) {
						resultValues[10] = Float8GetDatum(ubits(bitvec, 294, 8) / 10.0);
						nulls[10] = false;
					}

					if (type == 5) {
						tmp = uchars(bitvec, 302, 20, false);
						resultValues[11] = PointerGetDatum(cstring_to_text(tmp));
						pfree(tmp);

						nulls[11] = false;
					}

					if (type == 5) {
						i = (int32) ubits(bitvec, 274, 4);
						if (1 <= i && i <= 12) {
							resultValues[12] = Int32GetDatum(i);
							nulls[12] = false;
						}

						i = (int32) ubits(bitvec, 278, 5);
						if (1 <= i && i <= 31) {
							resultValues[13] = Int32GetDatum(i);
							nulls[13] = false;
						}

						i = (int32) ubits(bitvec, 283, 5);
						if (0 <= i && i <= 23) {
							resultValues[14] = Int32GetDatum(i);
							nulls[14] = false;
						}

						i = (int32) ubits(bitvec, 288, 6);
						if (0 <= i && i <= 59) {
							resultValues[15] = Int32GetDatum(i);
							nulls[15] = false;
						}
					}
				}
			}
		}
	}

	pfree(field[0]);
	pfree(ais);

	if (get_call_result_type(fcinfo, NULL, &resultTupleDesc) != TYPEFUNC_COMPOSITE)
		ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("function returning record called in context that cannot accept type record")));

	BlessTupleDesc(resultTupleDesc);
	result = heap_form_tuple(resultTupleDesc, resultValues, nulls);

	PG_RETURN_DATUM(HeapTupleGetDatum(result));
}
PG_FUNCTION_INFO_V1(decodeAISStaticData);
