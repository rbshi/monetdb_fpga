select splitpart(r'%Fz晟2墁', '', 1), splitpart(r'%Fz晟2墁', r'', 2), splitpart(r'%Fz晟2墁', r'', 1271186887);

START TRANSACTION; -- Bug 6916
CREATE TABLE "t0" ("c0" BOOLEAN NOT NULL,"c1" SMALLINT NOT NULL,CONSTRAINT "t0_c0_c1_pkey" PRIMARY KEY ("c0", "c1"));
INSERT INTO "t0" VALUES (false, -1);
create view v0(c0, c1, c2, c3) as (select ((t0.c1)<<(cast(0.09114074486978418487836961503489874303340911865234375 as int))), 0.4088967652609865, 0.3848869389602949109274732109042815864086151123046875, t0.c0 from t0 where t0.c0);

SELECT v0.c0 FROM t0 FULL OUTER JOIN v0 ON t0.c0;
	-- NULL
SELECT v0.c0 FROM t0 FULL OUTER JOIN v0 ON t0.c0 WHERE (rtrim(((upper(''))||(v0.c1)))) IS NULL;
	-- NULL
SELECT v0.c0 FROM t0 FULL OUTER JOIN v0 ON t0.c0 WHERE NOT ((rtrim(((upper(''))||(v0.c1)))) IS NULL);
	-- empty
SELECT v0.c0 FROM t0 FULL OUTER JOIN v0 ON t0.c0 WHERE ((rtrim(((upper(''))||(v0.c1)))) IS NULL) IS NULL;
	-- empty

ROLLBACK;

START TRANSACTION; -- Bug 6918
CREATE TABLE "sys"."t0" ("c0" BOOLEAN NOT NULL,"c1" BIGINT,CONSTRAINT "t0_c0_pkey" PRIMARY KEY ("c0"),CONSTRAINT "t0_c0_unique" UNIQUE ("c0"));
create view v0(c0, c1, c2) as (select all 2.020551048E9, 0.16688174, 0.3732000026221729 from t0 where t0.c0) with check option;
SELECT sql_min(sql_max(NULL, ''), '') FROM v0 LEFT OUTER JOIN t0 ON true;
SELECT sql_min(sql_max(NULL, ''), '');
SELECT ALL length(upper(MIN(ALL CAST(((trim(CAST(r'' AS STRING(659)), CAST(r'o3%+i]抔DCöf▟nßOpNbybಜ7' AS STRING)))||(sql_min(sql_max(NULL, r''), splitpart(r'x', r',7+.', t0.c1)))) AS STRING(151))))), 0.4179268710155164 
FROM v0 LEFT OUTER JOIN t0 ON NOT (t0.c0) WHERE t0.c0 GROUP BY 0.3584962, CAST(t0.c1 AS STRING(601)), t0.c1;
ROLLBACK;

START TRANSACTION; -- Bug 6919
CREATE TABLE "sys"."t0" (
	"c0" INTEGER       NOT NULL,
	"c1" DOUBLE,
	CONSTRAINT "t0_c0_pkey" PRIMARY KEY ("c0")
);
COPY 29 RECORDS INTO "sys"."t0" FROM stdin USING DELIMITERS E'\t',E'\n','"';
6	0.01926179604972278
7	0.01926179604972278
8	0.01926179604972278
9	0.01926179604972278
10	0.01926179604972278
11	0.01926179604972278
12	0.01926179604972278
13	0.01926179604972278
14	0.01926179604972278
15	0.01926179604972278
16	0.01926179604972278
17	0.01926179604972278
954233931	0.01926179604972278
-890980732	0.01926179604972278
18	0.9441921149477416
19	0.8647722974466762
20	0.6303259287607281
21	0.7198562388857971
22	1905034025
1927464158	0.827299544139285
421223489	0.03854140660184213
-906851618	0.01926179604972278
23	0.44641096314987394
24	0.5358519423727929
25	0.8490801972106654
911090097	1
-708085857	0.7843275143974144
26	1130231849
27	0.1052118441396751

select "insert"('屁{珙', 1, 1, '1'), "insert"('屁{珙', 1, 1, '抔'), "insert"('屁抔珙', 1, 1, 'ಜ'), "insert"('a', 0, 1, 'ಜ'), "insert"('a', 0, 0, 'ಜ');
select "insert"('屁{珙', 1, 1, '1'), "insert"('屁{珙', 1, 1, '抔'), "insert"('屁抔珙', 1, 1, 'ಜ') from t0;
ROLLBACK;

CREATE TABLE t0(c0 boolean, c1 boolean, c2 int, UNIQUE(c0, c1)); -- Bug 6920
INSERT INTO t0(c1) VALUES((0.5) NOT  BETWEEN SYMMETRIC (CAST(length(upper(r'z')) AS INT)) AND (2)), (FALSE);
INSERT INTO t0(c0) VALUES(TRUE);
INSERT INTO t0(c1) VALUES(FALSE), (TRUE);
INSERT INTO t0(c2, c0, c1) VALUES(2, (ltrim(lower(r'K'), ((upper(r'4'))||(- (7))))) IS NOT NULL, (((lower(r'ö
eg#K,纗HSJw!{cOw⇒l/l!B*H'))||(((CAST(0.1 AS INT))+(-5))))) IN (lower(CAST(TRUE AS STRING)))), (-9, NULL, (0.3) NOT IN (2.96348087E8, 0.3)), (4, TRUE, (0.5) NOT IN (-3, 4));
INSERT INTO t0(c1) VALUES(TRUE);
INSERT INTO t0(c1, c2) VALUES(FALSE, 623585248);
INSERT INTO t0(c0) VALUES(NOT (NOT ((((-1)*(-9))) NOT IN (0.7)))), (FALSE);
INSERT INTO t0(c0) VALUES(TRUE);
INSERT INTO t0(c2) VALUES(-1);
DELETE FROM t0 WHERE (t0.c0) = FALSE;
INSERT INTO t0(c1, c0, c2) VALUES(TRUE, ((+ (- (1)))>(charindex(CAST(0.3 AS STRING), ((CAST(TRUE AS STRING(771)))||(0.2)), length(r'd')))), -1);
UPDATE t0 SET c2 = 1 WHERE ((((t0.c1)OR(CAST(t0.c2 AS BOOLEAN))))OR((CAST(0.6 AS DECIMAL)) NOT IN (0.1)));
SELECT t0.c0 FROM t0 WHERE ((CAST(t0.c2 AS INT)) NOT  BETWEEN ASYMMETRIC (abs(t0.c2)) AND (((t0.c2)/(t0.c2)))) NOT  BETWEEN ASYMMETRIC (t0.c0) AND ((t0.c2) NOT IN (t0.c2));
SELECT t0.c0 FROM t0 WHERE (t0.c2) BETWEEN (t0.c0) AND ((t0.c2) NOT IN (t0.c2));
DROP TABLE t0;

CREATE TABLE "sys"."t0" (
	"c0" BOOLEAN,
	"c1" BOOLEAN,
	"c2" INTEGER,
	CONSTRAINT "t0_c2_pkey" PRIMARY KEY ("c2"),
	CONSTRAINT "t0_c0_c1_unique" UNIQUE ("c0", "c1")
);
INSERT INTO "sys"."t0" VALUES (NULL, true, 10),
 (NULL, false, 11),
 (true, NULL, 12),
 (NULL, false, 13),
 (NULL, true, 14),
 (true, false, 15),
 (NULL, true, 16),
 (true, true, 17),
 (NULL, true, 18),
 (NULL, false, 19),
 (true, NULL, 20),
 (true, NULL, 21),
 (NULL, NULL, 22),
 (false, true, 23);
TRUNCATE t0;

-- The following copy into shouldn't trigger a constrain violation error (it's the same insert above)
COPY 14 RECORDS INTO "sys"."t0" FROM stdin USING DELIMITERS E'\t',E'\n','"';
NULL	true	10
NULL	false	11
true	NULL	12
NULL	false	13
NULL	true	14
true	false	15
NULL	true	16
true	true	17
NULL	true	18
NULL	false	19
true	NULL	20
true	NULL	21
NULL	NULL	22
false	true	23

DROP TABLE t0;

START TRANSACTION;
CREATE TABLE "sys"."t0" (
	"c0" DECIMAL(18,3) NOT NULL DEFAULT cast(cast("sys"."sql_sub"("sys"."sql_neg"(1933820187),"sys"."charindex"(cast(0.235784 as clob(182)),"sys"."concat"('rt\\b&oOT',1174691962),"sys"."locate"('!oLqvKg恺','!G+tZ9\'A\'LZ*滻mW^',1174691962))) as int) as decimal(18,3)),
	"c1" DECIMAL(18,3),
	"c2" CHARACTER LARGE OBJECT,
	CONSTRAINT "t0_c0_pkey" PRIMARY KEY ("c0"),
	CONSTRAINT "t0_c0_unique" UNIQUE ("c0"),
	CONSTRAINT "t0_c2_unique" UNIQUE ("c2"),
	CONSTRAINT "t0_c0_c2_c1_unique" UNIQUE ("c0", "c2", "c1")
);
COMMENT ON COLUMN "sys"."t0"."c2" IS 'vi';
COPY 1 RECORDS INTO "sys"."t0" FROM stdin USING DELIMITERS E'\t',E'\n','"';
-1933820187.000	NULL	"-1554387152"

select t0.c0 from t0 where not (true) union all select all t0.c0 from t0 where not (not (true)) union all select t0.c0 from t0 where (not (true)) is null;
ROLLBACK;

START TRANSACTION;
CREATE TABLE t0 (c0 BOOLEAN);
create view v1(c0, c1, c2, c3) as (select 1, lower('1'), 1, cot(1) from t0);
SELECT v1.c0 FROM v1 WHERE v1.c1 LIKE v1.c1 AND v1.c3 BETWEEN v1.c3 AND v1.c3 = TRUE;

create view v0(c0, c1, c2, c3) as (select distinct 0.1, ((lower((((('&')||(2)))||(0.1))))||((('-1')||(abs(cast(r'Br' as int)))))), 0.1, cot(((((- (((2)&(3))))+(abs(4))))*(- (cast(cast(5 as string) as int))))) from t0);
SELECT v0.c0 FROM v0 WHERE NOT (NOT (((((((((NOT (((v0.c1)LIKE(v0.c1))))AND(((v0.c2)>=(v0.c0)))))AND(((v0.c3) BETWEEN SYMMETRIC (v0.c3) AND (v0.c3)) = TRUE)))AND
((((v0.c1)||(((v0.c0)<(v0.c0))))) BETWEEN SYMMETRIC (v0.c1) AND (v0.c1))))AND((0.2) BETWEEN SYMMETRIC (v0.c0) AND (v0.c0)))));
ROLLBACK;

START TRANSACTION;
CREATE TABLE t0(c0 INT);
insert into t0(c0) values(13), (((cast((0.1) between symmetric (0.2) and (0.3) as int))%(length(upper(r'u鋔wsD30pYi&ᴮ*2*W쉉t+詫sD^#cjX{'))))), (cast(cast(cast(0.12 as int) as string(646)) as int));

SELECT COUNT(t0.c0 = t0.c0 OR 1 BETWEEN ASYMMETRIC 1 AND 1) FROM t0;
SELECT 0.9, COUNT(ALL (((((((CAST(TRUE AS INT))-(t0.c0)))=(t0.c0))) = TRUE)OR((CAST(char_length(r'H') AS INT)) BETWEEN ASYMMETRIC (CAST(-1.3 AS INT)) AND (+ (length(r'0')))))), 0.3 FROM t0 GROUP BY 0.5, 0.4;
ROLLBACK;