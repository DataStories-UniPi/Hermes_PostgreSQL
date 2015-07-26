/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The file implements the loading functions of datasets
 *
 * @see @ref dataset_ais
 *
 */

/** @brief This function create the necessary tables for the Hermes MOD 
 *
 */
CREATE TABLE HDatasets (
	id serial NOT NULL,
	"name" text NOT NULL,
	name_long text NOT NULL,

	LRP PointLL,
	SRID integer,	--Use 3857 (Spherical Mercator) when the choice is not clear.
					--Bounds (lon, lat): min(-180, -85.0511), max(180, 85.0511)

	segment_storage boolean NOT NULL DEFAULT false,
	trajectory_storage boolean NOT NULL DEFAULT false,
	subtrajectory_storage boolean NOT NULL DEFAULT false,

	semantics_enabled boolean NOT NULL DEFAULT false,

	partitioning_t_range interval,
	partitioning_x_range integer,
	partitioning_y_range integer,

	notes text,

	PRIMARY KEY (id),
	UNIQUE ("name"),

	CHECK ((LRP IS NOT NULL AND SRID IS NULL) OR (SRID IS NOT NULL AND LRP IS NULL)),
	CHECK (NOT (SRID IS NOT NULL AND NOT (HParameters('postgis_integration') IS NOT NULL AND HParameters('postgis_integration')::boolean))),

	CHECK (segment_storage OR trajectory_storage OR subtrajectory_storage),
	CHECK ((NOT semantics_enabled) OR (semantics_enabled AND subtrajectory_storage))
);

/** @brief This function returns the ID of the dataset
 *
 *	@param[in] name the name of the dataset
 *
 *  @return the id of the dataset
 * 
 */
CREATE FUNCTION HDatasetID(name text) RETURNS integer AS
	'SELECT id FROM HDatasets WHERE name = $1;'
LANGUAGE SQL STABLE STRICT;

/** @brief This function returns the name of the dataset
 *
 *	@param[in] dataset the id of the dataset
 *
 *  @return the name of the dataset
 * 
 */
CREATE FUNCTION HDatasetName(dataset integer DEFAULT HCurrentDatasetID()) RETURNS text AS
	'SELECT name FROM HDatasets WHERE id = $1;'
LANGUAGE SQL STABLE STRICT;

/** @brief This function transform a PointSP to pointLL
 *
 *	@param[in] poi the PointSP for transformation
 *	@param[in] datasetID the id of the dataset that we will use to transform the point
 *
 *  @return the meters
 *   
 * Normally this function should have been inside PointLL declaration file 
 * but I am too bored to test the consequences of moving this function.
 * 
 */
CREATE FUNCTION PointLL(poi PointSP, datasetID integer DEFAULT HCurrentDatasetID()) RETURNS PointLL AS $$
DECLARE
	tmpLRP PointLL;
	tmpSRID integer;

	ret PointLL;
BEGIN
	SELECT LRP, SRID INTO tmpLRP, tmpSRID FROM HDatasets WHERE id = datasetID;
	IF tmpLRP IS NOT NULL THEN
		ret := PointLL(poi::PointXY, tmpLRP);
	ELSE
		ret := PointLLFromGeom(ST_Transform(ST_SetSRID(ST_MakePoint(getX(poi), getY(poi)), tmpSRID), 4326));
	END IF;

	RETURN ret;
END;
$$ LANGUAGE plpgsql STABLE STRICT;

/** @brief This function transform a PointLL to pointSP
 *
 *	@param[in] poi the PointLL for transformation
 *	@param[in] datasetID the id of the dataset that we will use to transform the point
 *
 *  @return the meters
 *   
 * Normally this function should have been inside PointLL declaration file 
 * but I am too bored to test the consequences of moving this function.
 * 
 */
CREATE FUNCTION PointSP(poi PointLL, datasetID integer DEFAULT HCurrentDatasetID()) RETURNS PointSP AS $$
DECLARE
	tmpLRP PointLL;
	tmpSRID integer;

	ret PointSP;
BEGIN
	SELECT LRP, SRID INTO tmpLRP, tmpSRID FROM HDatasets WHERE id = datasetID;
	IF tmpLRP IS NOT NULL THEN
		ret := PointXY(poi, tmpLRP)::PointSP;
	ELSE
		ret := PointSPFromGeom(ST_Transform(ST_SetSRID(ST_MakePoint(getLon(poi), getLat(poi)), 4326), tmpSRID));
	END IF;

	RETURN ret;
END;
$$ LANGUAGE plpgsql STABLE STRICT;

/** @brief This function deletes the dataset from the Hermes MOD
 *
 *	@param[in] dataset_name the name of the dataset
 *
 */
CREATE FUNCTION HDatasetsDrop(dataset_name text) RETURNS boolean AS $$
DECLARE
	dataset_segment_storage boolean;
	dataset_trajectory_storage boolean;
	dataset_subtrajectory_storage boolean;

	dataset_partitioning_t_range interval;
	dataset_partitioning_x_range integer;
	dataset_partitioning_y_range integer;

	r record;
BEGIN
	SELECT segment_storage, trajectory_storage, subtrajectory_storage, partitioning_t_range, partitioning_x_range, partitioning_y_range INTO dataset_segment_storage, dataset_trajectory_storage, dataset_subtrajectory_storage, dataset_partitioning_t_range, dataset_partitioning_x_range, dataset_partitioning_y_range FROM HDatasets WHERE name = dataset_name;
	IF NOT FOUND THEN
		RETURN false;
	END IF;

	IF dataset_segment_storage THEN
		IF dataset_partitioning_t_range IS NOT NULL THEN
			FOR r IN EXECUTE 'SELECT t_part FROM ' || quote_ident(dataset_name || '_seg_partitions') || ' ORDER BY t_part ASC;' LOOP
				EXECUTE 'DROP TABLE ' || quote_ident(dataset_name || '_seg_' || r.t_part) || ' CASCADE;';
			END LOOP;

			EXECUTE
				'
					DROP TABLE ' || quote_ident(dataset_name || '_seg_partitions') || ' CASCADE;
					DROP FUNCTION ' || quote_ident(dataset_name || '_seg_insert_trigger') || '() CASCADE;
				';
		END IF;

		EXECUTE
			'
				DROP TABLE ' || quote_ident(dataset_name || '_seg') || ' CASCADE;
				DROP TABLE ' || quote_ident(dataset_name || '_seg_lobby') || ' CASCADE;
			';
	END IF;

	IF dataset_subtrajectory_storage THEN
		EXECUTE 'DROP TABLE ' || quote_ident(dataset_name || '_subtraj') || ' CASCADE;';
	END IF;

	EXECUTE
		'
			DROP TABLE ' || quote_ident(dataset_name || '_traj') || ' CASCADE;
			DROP TABLE ' || quote_ident(dataset_name || '_obj') || ' CASCADE;
		';

	DELETE FROM HDatasets WHERE name = dataset_name;

	RETURN true;
END;
$$ LANGUAGE plpgsql STRICT;

/** @brief This function created the necessary tables that will
 * 	host the dateset
 *
 *	@param[in] dataset_name the short name of the dataset
 *	@param[in] dataset_long_name the description fo the dataset
 *	@param[in] dataset_LRP is a PointLL column that is the reference point for coordinate transformation
 *	@param[in] dataset_SRID is an integer column that contains the EPSG code of the projected reference system in which the dataset is stored in Hermes
 *	@param[in] dataset_segment_storage a boolean value that shows whether the segment storage is in use
 *	@param[in] dataset_trajectory_storage a boolean value shows whether the trajectory storage is in use
 *	@param[in] dataset_subtrajectory_storage 
 *	@param[in] dataset_semantics_enabled 
 *	@param[in] dataset_partitioning_t_range is not currently in use
 *	@param[in] dataset_partitioning_x_range is not currently in use
 *	@param[in] dataset_partitioning_y_range is not currently in use
 *	
 *	@return if the function was succesfull
 *
 *	@see @ref database_hdataset
 */
CREATE FUNCTION HDatasetsRegister(
	dataset_name text,
	dataset_name_long text,

	dataset_LRP PointLL DEFAULT NULL,
	dataset_SRID integer DEFAULT NULL,

	dataset_segment_storage boolean DEFAULT false,
	dataset_trajectory_storage boolean DEFAULT false,
	dataset_subtrajectory_storage boolean DEFAULT false,

	dataset_semantics_enabled boolean DEFAULT false,

	dataset_partitioning_t_range interval DEFAULT NULL,
	dataset_partitioning_x_range integer DEFAULT NULL,
	dataset_partitioning_y_range integer DEFAULT NULL
) RETURNS boolean AS $$
DECLARE
	query text;
BEGIN
	IF
		dataset_name IS NULL
		OR dataset_name_long IS NULL
		OR (dataset_LRP IS NOT NULL AND dataset_SRID IS NOT NULL)
		OR dataset_segment_storage IS NULL
		OR dataset_trajectory_storage IS NULL
		OR dataset_subtrajectory_storage IS NULL
		OR dataset_semantics_enabled IS NULL
		OR (dataset_segment_storage IS NULL AND dataset_trajectory_storage IS NULL AND dataset_subtrajectory_storage IS NULL)
		OR (NOT dataset_segment_storage AND NOT dataset_trajectory_storage AND NOT dataset_subtrajectory_storage)
		OR (dataset_semantics_enabled AND NOT dataset_subtrajectory_storage)
	THEN
		RETURN false;
	END IF;

	PERFORM HDatasetsDrop(dataset_name);

	IF dataset_LRP IS NULL AND dataset_SRID IS NULL THEN
		IF HParameters('postgis_integration')::boolean THEN
			dataset_SRID := 3857;
		ELSE
			dataset_LRP := PointLL(0, 0);
		END IF;
	END IF;

	INSERT INTO HDatasets(
		name, name_long, LRP, SRID,
		segment_storage, trajectory_storage, subtrajectory_storage, semantics_enabled,
		partitioning_t_range, partitioning_x_range, partitioning_y_range
	)
	VALUES (
		dataset_name, dataset_name_long, dataset_LRP, dataset_SRID,
		dataset_segment_storage, dataset_trajectory_storage, dataset_subtrajectory_storage, dataset_semantics_enabled,
		dataset_partitioning_t_range, dataset_partitioning_x_range, dataset_partitioning_y_range
	);

	EXECUTE
		'CREATE TABLE ' || quote_ident(dataset_name || '_obj') ||
		'(
			obj_id integer NOT NULL,

			PRIMARY KEY (obj_id)
		);';

	query := 'CREATE TABLE ' || quote_ident(dataset_name || '_traj') ||
		'(
			obj_id integer NOT NULL,
			traj_id integer NOT NULL';
	IF dataset_trajectory_storage THEN
		query := query || ',traj Trajectory';
	END IF;
	query := query || ',FOREIGN KEY (obj_id) REFERENCES ' || quote_ident(dataset_name || '_obj') || '(obj_id)
				ON DELETE CASCADE
				ON UPDATE CASCADE,
			PRIMARY KEY (obj_id, traj_id)
		);';
	EXECUTE query;

	IF dataset_segment_storage THEN
		EXECUTE
			'CREATE TABLE ' || quote_ident(dataset_name || '_seg') ||
			'(
				obj_id integer NOT NULL,
				traj_id integer NOT NULL,
				seg_id integer NOT NULL,

				seg SegmentST NOT NULL,

				FOREIGN KEY (obj_id, traj_id) REFERENCES ' || quote_ident(dataset_name || '_traj') || '(obj_id, traj_id)
					ON DELETE CASCADE
					ON UPDATE CASCADE,
				PRIMARY KEY (obj_id, traj_id, seg_id)
			);';

		EXECUTE
			'CREATE TABLE ' || quote_ident(dataset_name || '_seg_lobby') ||
			'(
				obj_id integer NOT NULL,
				traj_id integer NOT NULL,

				t timestamp NOT NULL,
				x integer NOT NULL,
				y integer NOT NULL,

				PRIMARY KEY (obj_id, traj_id)
			);';

		IF dataset_partitioning_t_range IS NOT NULL THEN
			EXECUTE
				'CREATE TABLE ' || quote_ident(dataset_name || '_seg_partitions') || ' (
					t_part bigint NOT NULL,

					PRIMARY KEY (t_part)
				);';

			EXECUTE
				'CREATE FUNCTION ' || quote_ident(dataset_name || '_seg_insert_trigger') || '() RETURNS trigger AS $BODY$
				DECLARE
					range double precision := ' || extract(epoch from dataset_partitioning_t_range) || ';
					first_part bigint;
					last_part bigint;
					i bigint;
					r record;
				BEGIN
					first_part := ceil(extract(epoch from getTi(NEW.seg)) / range);
					IF extract(epoch from getTi(NEW.seg)) = first_part * range THEN
						first_part := first_part + 1;
					END IF;

					last_part := ceil(extract(epoch from getTe(NEW.seg)) / range);

					FOR i IN first_part..last_part LOOP
						IF NOT exists(SELECT 1 FROM ' || quote_ident(dataset_name || '_seg_partitions') || ' WHERE t_part = i LIMIT 1) THEN
							EXECUTE ''CREATE TABLE '' || quote_ident(' || quote_literal(dataset_name) || ' || ''_seg_'' || i) || ''(
									FOREIGN KEY (obj_id, traj_id) REFERENCES ' || quote_ident(dataset_name || '_traj') || '(obj_id, traj_id)
										ON DELETE CASCADE
										ON UPDATE CASCADE,
									PRIMARY KEY (obj_id, traj_id, seg_id),

									CHECK (getTi(seg) >= '' || quote_literal((''epoch''::timestamp + ''1 second''::interval * ((i - 1) * range))::text) || '' AND getTe(seg) <= '' || quote_literal((''epoch''::timestamp + ''1 second''::interval * (i * range))::text) || '')
								) INHERITS (' || quote_ident(dataset_name || '_seg') || ');'';
							EXECUTE ''CREATE INDEX ON '' || quote_ident(' || quote_literal(dataset_name) || ' || ''_seg_'' || i) || '' USING gist (seg) WITH (FILLFACTOR = 100);'';
							INSERT INTO ' || quote_ident(dataset_name || '_seg_partitions') || '(t_part) VALUES (i);
						END IF;

						r := atPeriod(NEW.seg, intersection(getT(NEW.seg), Period(''epoch''::timestamp + ''1 second''::interval * ((i - 1) * range), ''epoch''::timestamp + ''1 second''::interval * (i * range))));
						EXECUTE ''INSERT INTO '' || quote_ident(' || quote_literal(dataset_name) || ' || ''_seg_'' || i) || ''(obj_id, traj_id, seg_id, seg)
							VALUES ($1, $2, $3, $4)'' USING NEW.obj_id, NEW.traj_id, NEW.seg_id, r.s;
					END LOOP;

					RETURN NULL;
				END;
				$BODY$ LANGUAGE plpgsql;';

			EXECUTE 'CREATE TRIGGER ' || quote_ident(dataset_name || '_seg_insert_trigger') || ' BEFORE INSERT ON ' || quote_ident(dataset_name || '_seg') || ' FOR EACH ROW EXECUTE PROCEDURE ' || quote_ident(dataset_name || '_seg_insert_trigger') || '();';
		END IF;
	END IF;

	IF dataset_subtrajectory_storage THEN
		query := 'CREATE TABLE ' || quote_ident(dataset_name || '_subtraj') ||
			'(
				obj_id integer NOT NULL,
				traj_id integer NOT NULL,
				subtraj_id integer NOT NULL,

				subtraj Trajectory NOT NULL';
		IF dataset_semantics_enabled THEN
			query := query || ',episode text NOT NULL,
				tags text NOT NULL';
		END IF;
		query := query || ',FOREIGN KEY (obj_id, traj_id) REFERENCES ' || quote_ident(dataset_name || '_traj') || '(obj_id, traj_id)
					ON DELETE CASCADE
					ON UPDATE CASCADE,
				PRIMARY KEY (obj_id, traj_id, subtraj_id)
			);';
		EXECUTE query;
	END IF;

	RETURN true;
END;
$$ LANGUAGE plpgsql;

/** @brief This function renames the dataset name
 * 
 *	@param[in] dataset_name the name of the dataset
 *	@param[in] dataset_name_new the new name of the dataset
 *
 */
CREATE FUNCTION HDatasetsRename(dataset_name text, dataset_name_new text) RETURNS boolean AS $$
DECLARE
	dataset_segment_storage boolean;
	dataset_trajectory_storage boolean;
	dataset_subtrajectory_storage boolean;

	dataset_partitioning_t_range interval;
	dataset_partitioning_x_range integer;
	dataset_partitioning_y_range integer;
BEGIN
	SELECT segment_storage, trajectory_storage, subtrajectory_storage, partitioning_t_range, partitioning_x_range, partitioning_y_range INTO dataset_segment_storage, dataset_trajectory_storage, dataset_subtrajectory_storage, dataset_partitioning_t_range, dataset_partitioning_x_range, dataset_partitioning_y_range FROM HDatasets WHERE name = dataset_name;
	IF NOT FOUND THEN
		RETURN false;
	END IF;

	IF dataset_partitioning_t_range IS NOT NULL OR dataset_partitioning_x_range IS NOT NULL OR dataset_partitioning_y_range IS NOT NULL THEN
		RETURN false;
	END IF;

	IF dataset_segment_storage THEN
		EXECUTE
			'
				ALTER TABLE ' || quote_ident(dataset_name || '_seg') || ' RENAME TO ' || quote_ident(dataset_name_new || '_seg') || ';
				ALTER TABLE ' || quote_ident(dataset_name || '_seg_lobby') || ' RENAME TO ' || quote_ident(dataset_name_new || '_seg_lobby') || ';
			';
	END IF;

	IF dataset_subtrajectory_storage THEN
		EXECUTE 'ALTER TABLE ' || quote_ident(dataset_name || '_subtraj') || ' RENAME TO ' || quote_ident(dataset_name_new || '_subtraj') || ';';
	END IF;

	EXECUTE
		'
			ALTER TABLE ' || quote_ident(dataset_name || '_traj') || ' RENAME TO ' || quote_ident(dataset_name_new || '_traj') || ';
			ALTER TABLE ' || quote_ident(dataset_name || '_obj') || ' RENAME TO ' || quote_ident(dataset_name_new || '_obj') || ';
		';

	UPDATE HDatasets
	SET name = dataset_name_new
	WHERE name = dataset_name;

	RETURN true;
END;
$$ LANGUAGE plpgsql STRICT;
