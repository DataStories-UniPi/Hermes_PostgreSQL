/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE EXTENSION pgcrypto;
CREATE EXTENSION hstore;

/******************************************************************************/
CREATE TABLE HParameters (
	key text NOT NULL,
	value text NOT NULL,

	PRIMARY KEY (key)
);

CREATE FUNCTION HParameters(keyIn text) RETURNS text AS
	'SELECT value FROM HParameters WHERE key = $1;'
LANGUAGE SQL STABLE STRICT;

CREATE FUNCTION HParameters(keyIn text, valueIn text) RETURNS void AS $$
BEGIN
	IF exists(SELECT 1 FROM HParameters WHERE key = keyIn LIMIT 1) THEN
		UPDATE HParameters SET value = valueIn WHERE key = keyIn;
	ELSE
		INSERT INTO HParameters(key, value) VALUES (keyIn, valueIn);
	END IF;
END;
$$ LANGUAGE plpgsql STRICT;

CREATE FUNCTION HParametersRemove(keyIn text) RETURNS void AS
	'DELETE FROM HParameters WHERE key = $1;'
LANGUAGE SQL STRICT;
