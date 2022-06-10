/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION S2T_TemporalBufferSize() RETURNS interval AS
	'$libdir/Hermes','getS2T_TemporalBufferSize'
LANGUAGE C;

CREATE FUNCTION S2T_TemporalBufferSize(S2T_TemporalBufferSize interval) RETURNS void AS
	'$libdir/Hermes','setS2T_TemporalBufferSize'
LANGUAGE C STRICT;

/******************************************************************************/
CREATE FUNCTION S2T_SpatialBufferSize() RETURNS integer AS
	'$libdir/Hermes','getS2T_SpatialBufferSize'
LANGUAGE C;

CREATE FUNCTION S2T_SpatialBufferSize(S2T_SpatialBufferSize integer) RETURNS void AS
	'$libdir/Hermes','setS2T_SpatialBufferSize'
LANGUAGE C STRICT;

/******************************************************************************/
CREATE FUNCTION S2T_SimplificationMethod() RETURNS text AS
	'$libdir/Hermes','getS2T_SimplificationMethod'
LANGUAGE C;

CREATE FUNCTION S2T_SimplificationMethodT(samplingPeriod interval) RETURNS void AS
	'$libdir/Hermes','setS2T_SimplificationMethodT'
LANGUAGE C STRICT;

CREATE FUNCTION S2T_SimplificationMethodDP(eps double precision) RETURNS void AS
	'$libdir/Hermes','setS2T_SimplificationMethodDP'
LANGUAGE C STRICT;

CREATE FUNCTION S2T_SimplificationMethodDPP(eps double precision) RETURNS void AS
	'$libdir/Hermes','setS2T_SimplificationMethodDPP'
LANGUAGE C STRICT;

CREATE FUNCTION S2T_SimplificationMethodNone() RETURNS void AS
	'$libdir/Hermes','setNoneS2T_SimplificationMethod'
LANGUAGE C;

/******************************************************************************/
CREATE FUNCTION S2T_VotingMethod() RETURNS text AS
	'$libdir/Hermes','getS2T_VotingMethod'
LANGUAGE C;

CREATE FUNCTION S2T_VotingMethod(S2T_VotingMethod text) RETURNS void AS
	'$libdir/Hermes','setS2T_VotingMethod'
LANGUAGE C STRICT;

CREATE FUNCTION S2T_VotingMethodNone() RETURNS void AS
	'$libdir/Hermes','setNoneS2T_VotingMethod'
LANGUAGE C;

/******************************************************************************/
CREATE FUNCTION S2T_Sigma() RETURNS double precision AS
	'$libdir/Hermes','getS2T_Sigma'
LANGUAGE C;

CREATE FUNCTION S2T_Sigma(S2T_Sigma double precision) RETURNS void AS
	'$libdir/Hermes','setS2T_Sigma'
LANGUAGE C STRICT;

/******************************************************************************/
CREATE FUNCTION S2T_VotingSignal()
RETURNS TABLE(normalized_voting double precision, normalized_lifespan interval) AS
	'$libdir/Hermes','getS2T_VotingSignal'
LANGUAGE C;

/******************************************************************************/
CREATE FUNCTION S2T_GiST_OPERATOR(segment SegmentST, trajectory Trajectory) RETURNS boolean AS
	'$libdir/Hermes','getFalse'
LANGUAGE C STRICT;

CREATE OPERATOR &&& (
	PROCEDURE = S2T_GiST_OPERATOR,
	LEFTARG = SegmentST,
	RIGHTARG = Trajectory
);

CREATE FUNCTION S2T_GiST_OPERATOR(segment SegmentST, box BoxST) RETURNS boolean AS
	'$libdir/Hermes','getFalse'
LANGUAGE C STRICT;

CREATE OPERATOR &&& (
	PROCEDURE = S2T_GiST_OPERATOR,
	LEFTARG = SegmentST,
	RIGHTARG = BoxST
);
