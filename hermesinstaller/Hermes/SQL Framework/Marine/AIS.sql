/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION computeAISChecksum(ais text) RETURNS integer AS
	'$libdir/Hermes','computeAISChecksumV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION isAuxiliaryMMSI(mmsi integer) RETURNS boolean AS
	'$libdir/Hermes','isAuxiliaryMMSIV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION decodeAISXML(ais text) RETURNS xml AS
	'$libdir/Hermes','decodeAISXML'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION decodeAIS(ais text,
	OUT type integer, OUT mmsi integer
) AS
	'$libdir/Hermes','decodeAIS'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION decodeAISPositionReport(ais text,
	OUT type integer, OUT mmsi integer,

	OUT status integer,

	--sensor indicator (such as a GPS and gyrocompass)
	OUT lon double precision, OUT lat double precision,
	OUT heading integer, OUT turn double precision,

	--calculated based on sensor indicator
	OUT speed double precision, OUT course double precision
) AS
	'$libdir/Hermes','decodeAISPositionReport'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION decodeAISStaticData(ais text,
	OUT type integer, OUT mmsi integer,

	--ship static information
	OUT imo integer, OUT callsign text, OUT shipname text,
	OUT to_bow integer, OUT to_stern integer, OUT to_port integer, OUT to_starboard integer,

	--trip information
	OUT shiptype integer, OUT draught double precision,
	OUT destination text, OUT month integer, OUT day integer, OUT hour integer, OUT minute integer
) AS
	'$libdir/Hermes','decodeAISStaticData'
LANGUAGE C IMMUTABLE STRICT;

--SELECT to_hex(computeAISChecksum('!AIVDM,1,1,,A,13u?etPv2;0n:dDPwUM1U1Cb069D,0*24')), substring('!AIVDM,1,1,,A,13u?etPv2;0n:dDPwUM1U1Cb069D,0*24' from position('*' in '!AIVDM,1,1,,A,13u?etPv2;0n:dDPwUM1U1Cb069D,0*24') + 1 for char_length('!AIVDM,1,1,,A,13u?etPv2;0n:dDPwUM1U1Cb069D,0*24'));
--SELECT to_hex(computeAISChecksum(message)) = substring(message from position('*' in message) + 1 for char_length(message)) AS same, to_hex(he_ais_compute_checksum(message)) AS checksum_computed, substring(message from position('*' in message) + 1 for char_length(message)) AS checksum_extracted FROM ais_message;
