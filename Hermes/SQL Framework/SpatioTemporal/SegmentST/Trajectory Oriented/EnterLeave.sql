/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION enter_leave(traj SegmentST[], box_area BoxSP,
	OUT enterPoint PointST, OUT leavePoint PointST
) AS $$
DECLARE
	nr_segments integer;
	intr_s SegmentSP;
	tmp PointST;
BEGIN
	nr_segments := array_length(traj, 1);

	FOR i IN 1..nr_segments LOOP
		SELECT s INTO intr_s FROM intersection(getSp(traj[i]), box_area, solid := false);

		IF intr_s IS NOT NULL THEN
			IF contains(box_area, getI(traj[i])) THEN
				IF leavePoint IS NULL THEN
					leavePoint := atPoint(traj[i], getE(intr_s), false);
				ELSE
					tmp := atPoint(traj[i], getE(intr_s), false);
					IF getT(tmp) > getT(leavePoint) THEN
						leavePoint := tmp;
					END IF;
				END IF;
			ELSIF contains(box_area, getE(traj[i])) THEN
				IF enterPoint IS NULL THEN
					enterPoint := atPoint(traj[i], getI(intr_s), false);
				ELSE
					tmp := atPoint(traj[i], getI(intr_s), false);
					IF getT(tmp) < getT(enterPoint) THEN
						enterPoint := tmp;
					END IF;
				END IF;
			ELSE
				IF enterPoint IS NULL THEN
					enterPoint := atPoint(traj[i], getI(intr_s), false);
				ELSE
					tmp := atPoint(traj[i], getI(intr_s), false);
					IF getT(tmp) < getT(enterPoint) THEN
						enterPoint := tmp;
					END IF;
				END IF;

				IF leavePoint IS NULL THEN
					leavePoint := atPoint(traj[i], getE(intr_s), false);
				ELSE
					tmp := atPoint(traj[i], getE(intr_s), false);
					IF getT(tmp) > getT(leavePoint) THEN
						leavePoint := tmp;
					END IF;
				END IF;
			END IF;
		END IF;
	END LOOP;
END;
$$ LANGUAGE plpgsql STABLE STRICT;
