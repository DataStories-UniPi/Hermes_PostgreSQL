/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION Period(trajectory Trajectory) RETURNS Period AS
	'$libdir/Hermes','Trajectory2PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (Trajectory AS Period) WITH FUNCTION Period(Trajectory) AS ASSIGNMENT;

/******************************************************************************/
CREATE FUNCTION BoxSP(trajectory Trajectory) RETURNS BoxSP AS
	'$libdir/Hermes','Trajectory2BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (Trajectory AS BoxSP) WITH FUNCTION BoxSP(Trajectory) AS ASSIGNMENT;

/******************************************************************************/
CREATE FUNCTION BoxST(trajectory Trajectory) RETURNS BoxST AS
	'$libdir/Hermes','Trajectory2BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (Trajectory AS BoxST) WITH FUNCTION BoxST(Trajectory) AS ASSIGNMENT;
