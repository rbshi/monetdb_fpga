START TRANSACTION;

CREATE TABLE tab0(col0 INTEGER, col1 INTEGER, col2 INTEGER);
CREATE TABLE tab1(col0 INTEGER, col1 INTEGER, col2 INTEGER);
CREATE TABLE tab2(col0 INTEGER, col1 INTEGER, col2 INTEGER);

INSERT INTO tab0 VALUES (97,1,99), (15,81,47), (87,21,10);
INSERT INTO tab1 VALUES (51,14,96), (85,5,59), (91,47,68);
INSERT INTO tab2 VALUES (64,77,40), (75,67,58), (46,51,23);

SELECT * FROM tab0 cor0 WHERE + col2 + + + 89 + - CAST ( NULL AS REAL ) NOT IN ( 28 * + 37 - ( + col1 ) + + 63, 4,
( ( + - col0 ) ), + col2 );

SELECT * FROM tab1 AS cor0 WHERE CAST ( NULL AS INTEGER ) * CAST ( CAST ( + col2 AS INTEGER ) AS INTEGER ) * + + col1
NOT IN ( + 79 + col1, - col2 );

SELECT DISTINCT * FROM tab2 WHERE + col2 / - + CAST ( NULL AS INTEGER ) NOT IN ( + 92 * + col1, 7 );

TRUNCATE tab0;
TRUNCATE tab1;
TRUNCATE tab2;

INSERT INTO tab0 VALUES (97,1,99), (15,81,47), (87,21,10);
INSERT INTO tab1 VALUES (51,14,96), (85,5,59), (91,47,68);
INSERT INTO tab2 VALUES (64,77,40), (75,67,58), (46,51,23);

SELECT * FROM tab0 WHERE + col1 * CAST ( NULL AS INTEGER ) NOT IN ( + col2 );

SELECT * FROM tab1 AS cor0 WHERE CAST ( + col1 AS INTEGER ) * - 49 NOT IN ( - - CAST ( NULL AS REAL ) * col1,
col2 - + 22, - - 21 );

SELECT DISTINCT * FROM tab2 AS cor0 WHERE CAST ( NULL AS INTEGER ) / col2 NOT IN ( 19 / col2 );

SELECT * FROM tab1, tab0 AS cor0 WHERE NULL NOT IN ( cor0.col2 ); --6 columns in the output

SELECT DISTINCT MAX ( DISTINCT CAST ( NULL AS INTEGER ) ) * COUNT ( * ) + COUNT ( * ) * + ( - COUNT ( * ) ) + - 
COUNT ( * ) + ( CAST ( NULL AS INTEGER ) ) + + MAX ( DISTINCT 20 ) / 55 + - COUNT ( * ) AS col1 
FROM tab1 AS cor0; --Single null value

ROLLBACK;
