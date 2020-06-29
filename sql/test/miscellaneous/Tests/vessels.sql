start transaction;
create table vessels (type int, id int, lat real, log real, nav_status int, sog int, rot int, timestamp timestamp, insertion_time timestamp);
CREATE VIEW distinct_vessels AS 
	SELECT type, id, lat, log, nav_status, sog, rot, timestamp, insertion_time from vessels
	WHERE (timestamp, id) IN (SELECT max(timestamp), id FROM vessels GROUP BY id);

CREATE FUNCTION distance_in_km_quick (p1_lat REAL, p1_log REAL, p2_lat REAL, p2_log REAL)
RETURNS REAL
BEGIN
	RETURN 111.319 * 
	SQRT(
		(p2_lat-p1_lat) * 
		(p2_lat-p1_lat) + 
		((p2_log-p1_log) * cos((p2_lat+p1_lat)*0.00872664626)) * 
		((p2_log-p1_log) * cos((p2_lat+p1_lat)*0.00872664626))
	);
END;

WITH distance AS (
		SELECT v1.id AS ship, v2.id AS neighbour_ship, distance_in_km_quick(v1.lat,v1.log,v2.lat,v2.log) AS distance_in_km 
		FROM distinct_vessels AS v1
		JOIN distinct_vessels AS v2 ON v1.id <> v2.id
	)
	SELECT current_timestamp AS calc_time, ship, neighbour_ship, distance_in_km FROM distance
	WHERE (distance_in_km) IN (SELECT min(distance_in_km) FROM distance GROUP BY ship);

WITH distance AS (
		SELECT v1.id AS ship, v2.id AS neighbour_ship, distance_in_km_quick(v1.lat,v1.log,v2.lat,v2.log) AS distance_in_km 
		FROM distinct_vessels AS v1
		JOIN distinct_vessels AS v2 ON v1.id <> v2.id
	)
	SELECT current_timestamp AS calc_time, ship, neighbour_ship, distance_in_km FROM distance
	WHERE (ship, distance_in_km) IN (SELECT ship, min(distance_in_km) FROM distance GROUP BY ship);
rollback;
