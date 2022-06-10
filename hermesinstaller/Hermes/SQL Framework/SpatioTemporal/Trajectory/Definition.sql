/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE Trajectory;

CREATE FUNCTION inTrajectory(cstring) RETURNS Trajectory AS
	'$libdir/Hermes','inTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outTrajectory(Trajectory) RETURNS cstring AS
	'$libdir/Hermes','outTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvTrajectory(internal) RETURNS Trajectory AS
	'$libdir/Hermes','recvTrajectory'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(Trajectory) RETURNS bytea AS
	'$libdir/Hermes','sendTrajectory'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE Trajectory (
	INPUT = inTrajectory,
	OUTPUT = outTrajectory,
	RECEIVE = recvTrajectory,
	SEND = send,

	INTERNALLENGTH = VARIABLE,
	ALIGNMENT = double,

	STORAGE = external
);

/******************************************************************************/
CREATE FUNCTION equals(trajectoryA Trajectory, trajectoryB Trajectory) RETURNS boolean AS
	'$libdir/Hermes','equalsTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(trajectory Trajectory) RETURNS integer AS
	'$libdir/Hermes','hashTrajectory'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = Trajectory,
	RIGHTARG = Trajectory,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS Trajectory_OPS DEFAULT FOR TYPE Trajectory USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(Trajectory);

/******************************************************************************/
CREATE FUNCTION clone(trajectory Trajectory) RETURNS Trajectory AS
	'$libdir/Hermes','cloneTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;
