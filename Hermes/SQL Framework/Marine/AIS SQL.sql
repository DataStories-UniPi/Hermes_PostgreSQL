/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION AISAttribute(doc xml, attribute text) RETURNS text AS $$
DECLARE
	ret text;
BEGIN
	SELECT XMLSERIALIZE(CONTENT tmp.unnest AS text)
		INTO ret
	FROM
		(
			SELECT unnest FROM unnest(xpath('/AIS/@' || attribute, doc)) LIMIT 1
		) tmp;

	RETURN ret;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISElement(doc xml, element text) RETURNS text AS $$
DECLARE
	ret text;
BEGIN
	SELECT XMLSERIALIZE(CONTENT tmp.unnest AS text)
		INTO ret
	FROM
		(
			SELECT unnest FROM unnest(xpath('/AIS/' || element || '/text()', doc)) LIMIT 1
		) tmp;

	RETURN ret;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

--SELECT AISAttribute(decodeAIS('!AIVDM,1,1,,A,13T=Qr00001d=8TEed0005Np0<2V,0*7D'), 'mmsi');

/******************************************************************************/
CREATE FUNCTION AISMessageType(messageType integer) RETURNS text AS $$
BEGIN
	CASE messageType
		WHEN 1 THEN RETURN 'Position Report Class A';
		WHEN 2 THEN RETURN 'Position Report Class A (Assigned schedule)';
		WHEN 3 THEN RETURN 'Position Report Class A (Response to interrogation)';
		WHEN 4 THEN RETURN 'Base Station Report';
		WHEN 5 THEN RETURN 'Static and Voyage Related Data';
		WHEN 6 THEN RETURN 'Binary Addressed Message';
		WHEN 7 THEN RETURN 'Binary Acknowledge';
		WHEN 8 THEN RETURN 'Binary Broadcast Message';
		WHEN 9 THEN RETURN 'Standard SAR Aircraft Position Report';
		WHEN 10 THEN RETURN 'UTC and Date Inquiry';
		WHEN 11 THEN RETURN 'UTC and Date Response';
		WHEN 12 THEN RETURN 'Addressed Safety Related Message';
		WHEN 13 THEN RETURN 'Safety Related Acknowledgement';
		WHEN 14 THEN RETURN 'Safety Related Broadcast Message';
		WHEN 15 THEN RETURN 'Interrogation';
		WHEN 16 THEN RETURN 'Assignment Mode Command';
		WHEN 17 THEN RETURN 'DGNSS Binary Broadcast Message';
		WHEN 18 THEN RETURN 'Standard Class B CS Position Report';
		WHEN 19 THEN RETURN 'Extended Class B Equipment Position Report';
		WHEN 20 THEN RETURN 'Data Link Management';
		WHEN 21 THEN RETURN 'Aid-to-Navigation Report';
		WHEN 22 THEN RETURN 'Channel Management';
		WHEN 23 THEN RETURN 'Group Assignment Command';
		WHEN 24 THEN RETURN 'Static Data Report';
		WHEN 25 THEN RETURN 'Single Slot Binary Message';
		WHEN 26 THEN RETURN 'Multiple Slot Binary Message With Communications State';
		WHEN 27 THEN RETURN 'Position Report For Long-Range Applications';
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISMessageTypeValidate(messageType integer) RETURNS integer AS $$
BEGIN
	CASE
		WHEN 1 <= messageType AND messageType <= 27 THEN RETURN messageType;
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISStatus(status integer) RETURNS text AS $$
BEGIN
	CASE status
		WHEN 0 THEN RETURN 'Under way using engine';
		WHEN 1 THEN RETURN 'At anchor';
		WHEN 2 THEN RETURN 'Not under command';
		WHEN 3 THEN RETURN 'Restricted manoeuverability';
		WHEN 4 THEN RETURN 'Constrained by her draught';
		WHEN 5 THEN RETURN 'Moored';
		WHEN 6 THEN RETURN 'Aground';
		WHEN 7 THEN RETURN 'Engaged in Fishing';
		WHEN 8 THEN RETURN 'Under way sailing';
		WHEN 9 THEN RETURN 'Reserved for future amendment of Navigational Status for HSC';
		WHEN 10 THEN RETURN 'Reserved for future amendment of Navigational Status for WIG';
		WHEN 11, 12, 13, 14 THEN RETURN 'Reserved for future use';
		WHEN 15 THEN RETURN 'Not defined (default)';
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISStatusValidate(status integer) RETURNS integer AS $$
BEGIN
	CASE
		WHEN 0 <= status AND status <= 15 THEN RETURN status;
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISTurn(turn integer) RETURNS text AS $$
BEGIN
	CASE
		WHEN turn = 0 THEN RETURN 'not turning';
		WHEN 0 < turn AND turn <= 126 THEN RETURN 'turning right at up to 708 degrees per minute or higher';
		WHEN -126 <= turn AND turn < 0 THEN RETURN 'turning left at up to 708 degrees per minute or higher';
		WHEN turn = 127 THEN RETURN 'turning right at more than 5deg/30s (No TI available)';
		WHEN turn = -127 THEN RETURN 'turning left at more than 5deg/30s (No TI available)';
		WHEN turn = 128 THEN RETURN 'indicates no turn information available (default)';
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISTurnValidate(turn integer)
RETURNS integer AS $$
BEGIN
	CASE
		WHEN -126 <= turn AND turn <= 126 THEN RETURN turn;
		WHEN turn = 127 THEN RETURN turn;
		WHEN turn = -127 THEN RETURN turn;
		WHEN turn = 128 THEN RETURN turn;
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISTurnValue(turn integer) RETURNS double precision AS $$
BEGIN
	IF turn = 0 THEN
		RETURN 0;
	ELSIF 0 < turn AND turn <= 126 THEN
		RETURN power(turn / 4.733, 2);
	ELSIF -126 <= turn AND turn < 0 THEN
		RETURN power(turn / 4.733, 2) * -1;
	ELSE
		RETURN NULL;
	END IF;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISSpeed(speed integer) RETURNS text AS $$
BEGIN
	CASE
		WHEN 0 <= speed AND speed <= 102 THEN RETURN speed || ' knots';
		WHEN speed = 1023 THEN RETURN 'speed is not available';
		WHEN speed = 1022 THEN RETURN '102.2 knots or higher';
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISSpeedValidate(speed integer) RETURNS integer AS $$
BEGIN
	CASE
		WHEN 0 <= speed AND speed <= 102 THEN RETURN speed;
		WHEN speed = 1023 THEN RETURN speed;
		WHEN speed = 1022 THEN RETURN speed;
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISAccuracy(accuracy boolean) RETURNS text AS $$
BEGIN
	CASE accuracy
		WHEN true THEN RETURN 'DGPS-quality fix with an accuracy of < 10ms';
		WHEN false THEN RETURN 'unaugmented GNSS fix with accuracy > 10m (default)';
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISLonValidate(lon double precision) RETURNS double precision AS $$
BEGIN
	CASE
		WHEN -180 <= lon AND lon <= 180 THEN RETURN lon;
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISLatValidate(lat double precision) RETURNS double precision AS $$
BEGIN
	CASE
		WHEN -90 <= lat AND lat <= 90 THEN RETURN lat;
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISCourseValidate(course integer) RETURNS integer AS $$
BEGIN
	CASE course
		WHEN 3600 THEN RETURN NULL;
		ELSE RETURN course;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISHeading(heading integer) RETURNS integer AS $$
BEGIN
	CASE
		WHEN 0 <= heading AND heading <= 359 THEN RETURN heading;
		WHEN heading = 511 THEN RETURN NULL;
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISHeadingValidate(heading integer) RETURNS integer AS $$
BEGIN
	CASE
		WHEN 0 <= heading AND heading <= 359 THEN RETURN heading;
		WHEN heading = 511 THEN RETURN heading;
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISSecond(second integer) RETURNS text AS $$
BEGIN
	CASE
		WHEN 0 <= second AND second <= 59 THEN RETURN 'seconds in UTC timestamp';
		WHEN second = 60 THEN RETURN 'time stamp is not available (default)';
		WHEN second = 61 THEN RETURN 'positioning system is in manual input mode';
		WHEN second = 62 THEN RETURN 'Electronic Position Fixing System operates in estimated (dead reckoning) mode';
		WHEN second = 63 THEN RETURN 'the positioning system is inoperative';
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISSecondValidate(second integer) RETURNS integer AS $$
BEGIN
	CASE
		WHEN 0 <= second AND second <= 63 THEN RETURN second;
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISSecondValue(second integer) RETURNS integer AS $$
BEGIN
	CASE
		WHEN 0 <= second AND second <= 59 THEN RETURN second;
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISManeuver(maneuver integer) RETURNS text AS $$
BEGIN
	CASE maneuver
		WHEN 0 THEN RETURN 'Not available (default)';
		WHEN 1 THEN RETURN 'No special maneuver';
		WHEN 2 THEN RETURN 'Special maneuver (such as regional passing arrangement)';
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISManeuverValidate(maneuver integer)
RETURNS integer AS $$
BEGIN
	CASE maneuver
		WHEN 0, 1, 2 THEN RETURN maneuver;
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISShiptype(shiptype integer) RETURNS text AS $$
BEGIN
	CASE shiptype
		WHEN 0 THEN RETURN 'Not available (default)';
		WHEN 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 THEN RETURN 'Reserved for future use';
		WHEN 20 THEN RETURN 'Wing in ground (WIG), all ships of this type';
		WHEN 21 THEN RETURN 'Wing in ground (WIG), Hazardous category A';
		WHEN 22 THEN RETURN 'Wing in ground (WIG), Hazardous category B';
		WHEN 23 THEN RETURN 'Wing in ground (WIG), Hazardous category C';
		WHEN 24 THEN RETURN 'Wing in ground (WIG), Hazardous category D';
		WHEN 25 THEN RETURN 'Wing in ground (WIG), Reserved for future use';
		WHEN 26 THEN RETURN 'Wing in ground (WIG), Reserved for future use';
		WHEN 27 THEN RETURN 'Wing in ground (WIG), Reserved for future use';
		WHEN 28 THEN RETURN 'Wing in ground (WIG), Reserved for future use';
		WHEN 29 THEN RETURN 'Wing in ground (WIG), Reserved for future use';
		WHEN 30 THEN RETURN 'Fishing';
		WHEN 31 THEN RETURN 'Towing';
		WHEN 32 THEN RETURN 'Towing: length exceeds 200m or breadth exceeds 25m';
		WHEN 33 THEN RETURN 'Dredging or underwater ops';
		WHEN 34 THEN RETURN 'Diving ops';
		WHEN 35 THEN RETURN 'Military ops';
		WHEN 36 THEN RETURN 'Sailing';
		WHEN 37 THEN RETURN 'Pleasure Craft';
		WHEN 38 THEN RETURN 'Reserved';
		WHEN 39 THEN RETURN 'Reserved';
		WHEN 40 THEN RETURN 'High speed craft (HSC), all ships of this type';
		WHEN 41 THEN RETURN 'High speed craft (HSC), Hazardous category A';
		WHEN 42 THEN RETURN 'High speed craft (HSC), Hazardous category B';
		WHEN 43 THEN RETURN 'High speed craft (HSC), Hazardous category C';
		WHEN 44 THEN RETURN 'High speed craft (HSC), Hazardous category D';
		WHEN 45 THEN RETURN 'High speed craft (HSC), Reserved for future use';
		WHEN 46 THEN RETURN 'High speed craft (HSC), Reserved for future use';
		WHEN 47 THEN RETURN 'High speed craft (HSC), Reserved for future use';
		WHEN 48 THEN RETURN 'High speed craft (HSC), Reserved for future use';
		WHEN 49 THEN RETURN 'High speed craft (HSC), No additional information';
		WHEN 50 THEN RETURN 'Pilot Vessel';
		WHEN 51 THEN RETURN 'Search and Rescue vessel';
		WHEN 52 THEN RETURN 'Tug';
		WHEN 53 THEN RETURN 'Port Tender';
		WHEN 54 THEN RETURN 'Anti-pollution equipment';
		WHEN 55 THEN RETURN 'Law Enforcement';
		WHEN 56 THEN RETURN 'Spare - Local Vessel';
		WHEN 57 THEN RETURN 'Spare - Local Vessel';
		WHEN 58 THEN RETURN 'Medical Transport';
		WHEN 59 THEN RETURN 'Ship according to RR Resolution No. 18';
		WHEN 60 THEN RETURN 'Passenger, all ships of this type';
		WHEN 61 THEN RETURN 'Passenger, Hazardous category A';
		WHEN 62 THEN RETURN 'Passenger, Hazardous category B';
		WHEN 63 THEN RETURN 'Passenger, Hazardous category C';
		WHEN 64 THEN RETURN 'Passenger, Hazardous category D';
		WHEN 65 THEN RETURN 'Passenger, Reserved for future use';
		WHEN 66 THEN RETURN 'Passenger, Reserved for future use';
		WHEN 67 THEN RETURN 'Passenger, Reserved for future use';
		WHEN 68 THEN RETURN 'Passenger, Reserved for future use';
		WHEN 69 THEN RETURN 'Passenger, No additional information';
		WHEN 70 THEN RETURN 'Cargo, all ships of this type';
		WHEN 71 THEN RETURN 'Cargo, Hazardous category A';
		WHEN 72 THEN RETURN 'Cargo, Hazardous category B';
		WHEN 73 THEN RETURN 'Cargo, Hazardous category C';
		WHEN 74 THEN RETURN 'Cargo, Hazardous category D';
		WHEN 75 THEN RETURN 'Cargo, Reserved for future use';
		WHEN 76 THEN RETURN 'Cargo, Reserved for future use';
		WHEN 77 THEN RETURN 'Cargo, Reserved for future use';
		WHEN 78 THEN RETURN 'Cargo, Reserved for future use';
		WHEN 79 THEN RETURN 'Cargo, No additional information';
		WHEN 80 THEN RETURN 'Tanker, all ships of this type';
		WHEN 81 THEN RETURN 'Tanker, Hazardous category A';
		WHEN 82 THEN RETURN 'Tanker, Hazardous category B';
		WHEN 83 THEN RETURN 'Tanker, Hazardous category C';
		WHEN 84 THEN RETURN 'Tanker, Hazardous category D';
		WHEN 85 THEN RETURN 'Tanker, Reserved for future use';
		WHEN 86 THEN RETURN 'Tanker, Reserved for future use';
		WHEN 87 THEN RETURN 'Tanker, Reserved for future use';
		WHEN 88 THEN RETURN 'Tanker, Reserved for future use';
		WHEN 89 THEN RETURN 'Tanker, No additional information';
		WHEN 90 THEN RETURN 'Other Type, all ships of this type';
		WHEN 91 THEN RETURN 'Other Type, Hazardous category A';
		WHEN 92 THEN RETURN 'Other Type, Hazardous category B';
		WHEN 93 THEN RETURN 'Other Type, Hazardous category C';
		WHEN 94 THEN RETURN 'Other Type, Hazardous category D';
		WHEN 95 THEN RETURN 'Other Type, Reserved for future use';
		WHEN 96 THEN RETURN 'Other Type, Reserved for future use';
		WHEN 97 THEN RETURN 'Other Type, Reserved for future use';
		WHEN 98 THEN RETURN 'Other Type, Reserved for future use';
		WHEN 99 THEN RETURN 'Other Type, no additional information';
		ELSE RETURN 'Not available';
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISEpfd(epfd integer) RETURNS text AS $$
BEGIN
	CASE epfd
		WHEN 0 THEN RETURN 'Undefined (default)';
		WHEN 1 THEN RETURN 'GPS';
		WHEN 2 THEN RETURN 'GLONASS';
		WHEN 3 THEN RETURN 'Combined GPS/GLONASS';
		WHEN 4 THEN RETURN 'Loran-C';
		WHEN 5 THEN RETURN 'Chayka';
		WHEN 6 THEN RETURN 'Integrated navigation system';
		WHEN 7 THEN RETURN 'Surveyed';
		WHEN 8 THEN RETURN 'Galileo';
		WHEN 9 THEN RETURN 'Undefined';
		WHEN 10 THEN RETURN 'Undefined';
		WHEN 11 THEN RETURN 'Undefined';
		WHEN 12 THEN RETURN 'Undefined';
		WHEN 13 THEN RETURN 'Undefined';
		WHEN 14 THEN RETURN 'Undefined';
		WHEN 15 THEN RETURN 'Undefined';
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISEpfdValidate(epfd integer) RETURNS integer AS $$
BEGIN
	CASE
		WHEN 0 <= epfd AND epfd <= 15 THEN RETURN epfd;
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISToBow(toBow integer) RETURNS text AS $$
BEGIN
	CASE toBow
		WHEN 0 THEN RETURN 'not available';
		WHEN 511 THEN RETURN '511 meters or greater';
		ELSE RETURN toBow || ' meters';
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISToStern(toStern integer) RETURNS text AS $$
BEGIN
	CASE toStern
		WHEN 0 THEN RETURN 'not available';
		WHEN 511 THEN RETURN '511 meters or greater';
		ELSE RETURN toStern || ' meters';
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISToPort(toPort integer) RETURNS text AS $$
BEGIN
	CASE toPort
		WHEN 0 THEN RETURN 'not available';
		WHEN 63 THEN RETURN '63 meters or greater';
		ELSE RETURN toPort || ' meters';
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISToStarboard(toStarboard integer) RETURNS text AS $$
BEGIN
	CASE toStarboard
		WHEN 0 THEN RETURN 'not available';
		WHEN 63 THEN RETURN '63 meters or greater';
		ELSE RETURN toStarboard || ' meters';
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISFlag(mmsi integer) RETURNS text AS $$
BEGIN
	CASE (mmsi / 1000000)::integer
		WHEN 201 THEN RETURN 'Albania (Republic of)';
		WHEN 202 THEN RETURN 'Andorra (Principality of)';
		WHEN 203 THEN RETURN 'Austria';
		WHEN 204 THEN RETURN 'Azores';
		WHEN 205 THEN RETURN 'Belgium';
		WHEN 206 THEN RETURN 'Belarus (Republic of)';
		WHEN 207 THEN RETURN 'Bulgaria (Republic of)';
		WHEN 208 THEN RETURN 'Vatican City State';
		WHEN 209, 210 THEN RETURN 'Cyprus (Republic of)';
		WHEN 211 THEN RETURN 'Germany (Federal Republic of)';
		WHEN 212 THEN RETURN 'Cyprus (Republic of)';
		WHEN 213 THEN RETURN 'Georgia';
		WHEN 214 THEN RETURN 'Moldova (Republic of)';
		WHEN 215 THEN RETURN 'Malta';
		WHEN 216 THEN RETURN 'Armenia (Republic of)';
		WHEN 218 THEN RETURN 'Germany (Federal Republic of)';
		WHEN 219, 220 THEN RETURN 'Denmark';
		WHEN 224, 225 THEN RETURN 'Spain';
		WHEN 226, 227, 228 THEN RETURN 'France';
		WHEN 230 THEN RETURN 'Finland';
		WHEN 231 THEN RETURN 'Faroe Islands';
		WHEN 232, 233, 234, 235 THEN RETURN 'United Kingdom of Great Britain and Northern Ireland';
		WHEN 236 THEN RETURN 'Gibraltar';
		WHEN 237 THEN RETURN 'Greece';
		WHEN 238 THEN RETURN 'Croatia (Republic of)';
		WHEN 239, 240, 241 THEN RETURN 'Greece';
		WHEN 242 THEN RETURN 'Morocco (Kingdom of)';
		WHEN 243 THEN RETURN 'Hungary (Republic of)';
		WHEN 244, 245, 246 THEN RETURN 'Netherlands (Kingdom of the)';
		WHEN 247 THEN RETURN 'Italy';
		WHEN 248, 249 THEN RETURN 'Malta';
		WHEN 250 THEN RETURN 'Ireland';
		WHEN 251 THEN RETURN 'Iceland';
		WHEN 252 THEN RETURN 'Liechtenstein (Principality of)';
		WHEN 253 THEN RETURN 'Luxembourg';
		WHEN 254 THEN RETURN 'Monaco (Principality of)';
		WHEN 255 THEN RETURN 'Madeira';
		WHEN 256 THEN RETURN 'Malta';
		WHEN 257, 258, 259 THEN RETURN 'Norway';
		WHEN 261 THEN RETURN 'Poland (Republic of)';
		WHEN 262 THEN RETURN 'Montenegro';
		WHEN 263 THEN RETURN 'Portugal';
		WHEN 264 THEN RETURN 'Romania';
		WHEN 265, 266 THEN RETURN 'Sweden';
		WHEN 267 THEN RETURN 'Slovak Republic';
		WHEN 268 THEN RETURN 'San Marino (Republic of)';
		WHEN 269 THEN RETURN 'Switzerland (Confederation of)';
		WHEN 270 THEN RETURN 'Czech Republic';
		WHEN 271 THEN RETURN 'Turkey';
		WHEN 272 THEN RETURN 'Ukraine';
		WHEN 273 THEN RETURN 'Russian Federation';
		WHEN 274 THEN RETURN 'The Former Yugoslav Republic of Macedonia';
		WHEN 275 THEN RETURN 'Latvia (Republic of)';
		WHEN 276 THEN RETURN 'Estonia (Republic of)';
		WHEN 277 THEN RETURN 'Lithuania (Republic of)';
		WHEN 278 THEN RETURN 'Slovenia (Republic of)';
		WHEN 279 THEN RETURN 'Serbia (Republic of)';
		WHEN 301 THEN RETURN 'Anguilla';
		WHEN 303 THEN RETURN 'Alaska (State of)';
		WHEN 304, 305 THEN RETURN 'Antigua and Barbuda';
		WHEN 306 THEN RETURN 'Netherlands Caribbean';
		WHEN 307 THEN RETURN 'Aruba';
		WHEN 308, 309 THEN RETURN 'Bahamas (Commonwealth of the)';
		WHEN 310 THEN RETURN 'Bermuda';
		WHEN 311 THEN RETURN 'Bahamas (Commonwealth of the)';
		WHEN 312 THEN RETURN 'Belize';
		WHEN 314 THEN RETURN 'Barbados';
		WHEN 316 THEN RETURN 'Canada';
		WHEN 319 THEN RETURN 'Cayman Islands';
		WHEN 321 THEN RETURN 'Costa Rica';
		WHEN 323 THEN RETURN 'Cuba';
		WHEN 325 THEN RETURN 'Dominica (Commonwealth of)';
		WHEN 327 THEN RETURN 'Dominican Republic';
		WHEN 329 THEN RETURN 'France';
		WHEN 330 THEN RETURN 'Grenada';
		WHEN 331 THEN RETURN 'Greenland';
		WHEN 332 THEN RETURN 'Guatemala (Republic of)';
		WHEN 334 THEN RETURN 'Honduras (Republic of)';
		WHEN 336 THEN RETURN 'Haiti (Republic of)';
		WHEN 338 THEN RETURN 'United States of America';
		WHEN 339 THEN RETURN 'Jamaica';
		WHEN 341 THEN RETURN 'Saint Kitts and Nevis (Federation of)';
		WHEN 343 THEN RETURN 'Saint Lucia';
		WHEN 345 THEN RETURN 'Mexico';
		WHEN 347 THEN RETURN 'Martinique (French Department of)';
		WHEN 348 THEN RETURN 'Montserrat';
		WHEN 350 THEN RETURN 'Nicaragua';
		WHEN 351, 352, 353, 354, 355, 356, 357 THEN RETURN 'Panama (Republic of)';
		WHEN 358 THEN RETURN 'Puerto Rico';
		WHEN 359 THEN RETURN 'El Salvador (Republic of)';
		WHEN 361 THEN RETURN 'Saint Pierre and Miquelon (Territorial Collectivity of)';
		WHEN 362 THEN RETURN 'Trinidad and Tobago';
		WHEN 364 THEN RETURN 'Turks and Caicos Islands';
		WHEN 366, 367, 368, 369 THEN RETURN 'United States of America';
		WHEN 370, 371, 372 THEN RETURN 'Panama (Republic of)';
		WHEN 375, 376, 377 THEN RETURN 'Saint Vincent and the Grenadines';
		WHEN 378 THEN RETURN 'British Virgin Islands';
		WHEN 379 THEN RETURN 'United States Virgin Islands';
		WHEN 401 THEN RETURN 'Afghanistan';
		WHEN 403 THEN RETURN 'Saudi Arabia (Kingdom of)';
		WHEN 405 THEN RETURN 'Bangladesh (People''s Republic of)';
		WHEN 408 THEN RETURN 'Bahrain (Kingdom of)';
		WHEN 410 THEN RETURN 'Bhutan (Kingdom of)';
		WHEN 412, 413, 414 THEN RETURN 'China (People''s Republic of)';
		WHEN 416 THEN RETURN 'Taiwan (Province of China)';
		WHEN 417 THEN RETURN 'Sri Lanka (Democratic Socialist Republic of)';
		WHEN 419 THEN RETURN 'India (Republic of)';
		WHEN 422 THEN RETURN 'Iran (Islamic Republic of)';
		WHEN 423 THEN RETURN 'Azerbaijani Republic';
		WHEN 425 THEN RETURN 'Iraq (Republic of)';
		WHEN 428 THEN RETURN 'Israel (State of)';
		WHEN 431, 432 THEN RETURN 'Japan';
		WHEN 434 THEN RETURN 'Turkmenistan';
		WHEN 436 THEN RETURN 'Kazakhstan (Republic of)';
		WHEN 437 THEN RETURN 'Uzbekistan (Republic of)';
		WHEN 438 THEN RETURN 'Jordan (Hashemite Kingdom of)';
		WHEN 440, 441 THEN RETURN 'Korea (Republic of)';
		WHEN 443 THEN RETURN 'Palestine (In accordance with Resolution 99 Rev. Guadalajara, 2010)';
		WHEN 445 THEN RETURN 'Democratic People''s Republic of Korea';
		WHEN 447 THEN RETURN 'Kuwait (State of)';
		WHEN 450 THEN RETURN 'Lebanon';
		WHEN 451 THEN RETURN 'Kyrgyz Republic';
		WHEN 453 THEN RETURN 'Macao (Special Administrative Region of China)';
		WHEN 455 THEN RETURN 'Maldives (Republic of)';
		WHEN 457 THEN RETURN 'Mongolia';
		WHEN 459 THEN RETURN 'Nepal (Federal Democratic Republic of)';
		WHEN 461 THEN RETURN 'Oman (Sultanate of)';
		WHEN 463 THEN RETURN 'Pakistan (Islamic Republic of)';
		WHEN 466 THEN RETURN 'Qatar (State of)';
		WHEN 468 THEN RETURN 'Syrian Arab Republic';
		WHEN 470 THEN RETURN 'United Arab Emirates';
		WHEN 473, 475 THEN RETURN 'Yemen (Republic of)';
		WHEN 477 THEN RETURN 'Hong Kong (Special Administrative Region of China)';
		WHEN 478 THEN RETURN 'Bosnia and Herzegovina';
		WHEN 501 THEN RETURN 'Adelie Land';
		WHEN 503 THEN RETURN 'Australia';
		WHEN 506 THEN RETURN 'Myanmar (Union of)';
		WHEN 508 THEN RETURN 'Brunei Darussalam';
		WHEN 510 THEN RETURN 'Micronesia (Federated States of)';
		WHEN 511 THEN RETURN 'Palau (Republic of)';
		WHEN 512 THEN RETURN 'New Zealand';
		WHEN 514, 515 THEN RETURN 'Cambodia (Kingdom of)';
		WHEN 516 THEN RETURN 'Christmas Island (Indian Ocean)';
		WHEN 518 THEN RETURN 'Cook Islands';
		WHEN 520 THEN RETURN 'Fiji (Republic of)';
		WHEN 523 THEN RETURN 'Cocos (Keeling) Islands';
		WHEN 525 THEN RETURN 'Indonesia (Republic of)';
		WHEN 529 THEN RETURN 'Kiribati (Republic of)';
		WHEN 531 THEN RETURN 'Lao People''s Democratic Republic';
		WHEN 533 THEN RETURN 'Malaysia';
		WHEN 536 THEN RETURN 'Northern Mariana Islands (Commonwealth of the)';
		WHEN 538 THEN RETURN 'Marshall Islands (Republic of the)';
		WHEN 540 THEN RETURN 'New Caledonia';
		WHEN 542 THEN RETURN 'Niue';
		WHEN 544 THEN RETURN 'Nauru (Republic of)';
		WHEN 546 THEN RETURN 'French Polynesia';
		WHEN 548 THEN RETURN 'Philippines (Republic of the)';
		WHEN 553 THEN RETURN 'Papua New Guinea';
		WHEN 555 THEN RETURN 'Pitcairn Island';
		WHEN 557 THEN RETURN 'Solomon Islands';
		WHEN 559 THEN RETURN 'American Samoa';
		WHEN 561 THEN RETURN 'Samoa (Independent State of)';
		WHEN 563, 564, 565, 566 THEN RETURN 'Singapore (Republic of)';
		WHEN 567 THEN RETURN 'Thailand';
		WHEN 570 THEN RETURN 'Tonga (Kingdom of)';
		WHEN 572 THEN RETURN 'Tuvalu';
		WHEN 574 THEN RETURN 'Viet Nam (Socialist Republic of)';
		WHEN 576 THEN RETURN 'Vanuatu (Republic of)';
		WHEN 578 THEN RETURN 'Wallis and Futuna Islands';
		WHEN 601 THEN RETURN 'South Africa (Republic of)';
		WHEN 603 THEN RETURN 'Angola (Republic of)';
		WHEN 605 THEN RETURN 'Algeria (People''s Democratic Republic of)';
		WHEN 607 THEN RETURN 'Saint Paul and Amsterdam Islands';
		WHEN 608 THEN RETURN 'Ascension Island';
		WHEN 609 THEN RETURN 'Burundi (Republic of)';
		WHEN 610 THEN RETURN 'Benin (Republic of)';
		WHEN 611 THEN RETURN 'Botswana (Republic of)';
		WHEN 612 THEN RETURN 'Central African Republic';
		WHEN 613 THEN RETURN 'Cameroon (Republic of)';
		WHEN 615 THEN RETURN 'Congo (Republic of the)';
		WHEN 616 THEN RETURN 'Comoros (Union of the)';
		WHEN 617 THEN RETURN 'Cape Verde (Republic of)';
		WHEN 618 THEN RETURN 'Crozet Archipelago';
		WHEN 619 THEN RETURN 'Cote d''Ivoire (Republic of)';
		WHEN 621 THEN RETURN 'Djibouti (Republic of)';
		WHEN 622 THEN RETURN 'Egypt (Arab Republic of)';
		WHEN 624 THEN RETURN 'Ethiopia (Federal Democratic Republic of)';
		WHEN 625 THEN RETURN 'Eritrea';
		WHEN 626 THEN RETURN 'Gabonese Republic';
		WHEN 627 THEN RETURN 'Ghana';
		WHEN 629 THEN RETURN 'Gambia (Republic of the)';
		WHEN 630 THEN RETURN 'Guinea-Bissau (Republic of)';
		WHEN 631 THEN RETURN 'Equatorial Guinea (Republic of)';
		WHEN 632 THEN RETURN 'Guinea (Republic of)';
		WHEN 633 THEN RETURN 'Burkina Faso';
		WHEN 634 THEN RETURN 'Kenya (Republic of)';
		WHEN 635 THEN RETURN 'Kerguelen Islands';
		WHEN 636, 637 THEN RETURN 'Liberia (Republic of)';
		WHEN 642 THEN RETURN 'Socialist People''s Libyan Arab Jamahiriya';
		WHEN 644 THEN RETURN 'Lesotho (Kingdom of)';
		WHEN 645 THEN RETURN 'Mauritius (Republic of)';
		WHEN 647 THEN RETURN 'Madagascar (Republic of)';
		WHEN 649 THEN RETURN 'Mali (Republic of)';
		WHEN 650 THEN RETURN 'Mozambique (Republic of)';
		WHEN 654 THEN RETURN 'Mauritania (Islamic Republic of)';
		WHEN 655 THEN RETURN 'Malawi';
		WHEN 656 THEN RETURN 'Niger (Republic of the)';
		WHEN 657 THEN RETURN 'Nigeria (Federal Republic of)';
		WHEN 659 THEN RETURN 'Namibia (Republic of)';
		WHEN 660 THEN RETURN 'Reunion (French Department of)';
		WHEN 661 THEN RETURN 'Rwanda (Republic of)';
		WHEN 662 THEN RETURN 'Sudan (Republic of the)';
		WHEN 663 THEN RETURN 'Senegal (Republic of)';
		WHEN 664 THEN RETURN 'Seychelles (Republic of)';
		WHEN 665 THEN RETURN 'Saint Helena';
		WHEN 666 THEN RETURN 'Somali Democratic Republic';
		WHEN 667 THEN RETURN 'Sierra Leone';
		WHEN 668 THEN RETURN 'Sao Tome and Principe (Democratic Republic of)';
		WHEN 669 THEN RETURN 'Swaziland (Kingdom of)';
		WHEN 670 THEN RETURN 'Chad (Republic of)';
		WHEN 671 THEN RETURN 'Togolese Republic';
		WHEN 672 THEN RETURN 'Tunisia';
		WHEN 674 THEN RETURN 'Tanzania (United Republic of)';
		WHEN 675 THEN RETURN 'Uganda (Republic of)';
		WHEN 676 THEN RETURN 'Democratic Republic of the Congo';
		WHEN 677 THEN RETURN 'Tanzania (United Republic of)';
		WHEN 678 THEN RETURN 'Zambia (Republic of)';
		WHEN 679 THEN RETURN 'Zimbabwe (Republic of)';
		WHEN 701 THEN RETURN 'Argentine Republic';
		WHEN 710 THEN RETURN 'Brazil (Federative Republic of)';
		WHEN 720 THEN RETURN 'Bolivia (Plurinational State of)';
		WHEN 725 THEN RETURN 'Chile';
		WHEN 730 THEN RETURN 'Colombia (Republic of)';
		WHEN 735 THEN RETURN 'Ecuador';
		WHEN 740 THEN RETURN 'Falkland Islands (Malvinas)';
		WHEN 745 THEN RETURN 'Guiana (French Department of)';
		WHEN 750 THEN RETURN 'Guyana';
		WHEN 755 THEN RETURN 'Paraguay (Republic of)';
		WHEN 760 THEN RETURN 'Peru';
		WHEN 765 THEN RETURN 'Suriname (Republic of)';
		WHEN 770 THEN RETURN 'Uruguay (Eastern Republic of)';
		WHEN 775 THEN RETURN 'Venezuela (Bolivarian Republic of)';
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION AISIMOValidate(imo integer) RETURNS integer AS $$
BEGIN
	CASE
		WHEN length(imo::text) = 7 AND right((
				substring(imo::text from 1 for 1)::integer * 7
				+ substring(imo::text from 2 for 1)::integer * 6
				+ substring(imo::text from 3 for 1)::integer * 5
				+ substring(imo::text from 4 for 1)::integer * 4
				+ substring(imo::text from 5 for 1)::integer * 3
				+ substring(imo::text from 6 for 1)::integer * 2
			)::text, 1)::integer = substring(imo::text from 7 for 1)::integer THEN RETURN imo;
		ELSE RETURN NULL;
	END CASE;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;
