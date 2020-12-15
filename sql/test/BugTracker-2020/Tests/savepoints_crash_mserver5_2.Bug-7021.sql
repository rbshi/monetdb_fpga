-- triggers: sql/storage/store.c:3940: trans_init: Assertion `istmp || !c->base.allocated' failed.
start transaction;
create table savepointtest (id int, primary key(id));
savepoint name1;
insert into savepointtest values(1), (2), (3);
--savepoint name2;
insert into savepointtest values(4), (5), (6);
insert into savepointtest values(7), (8), (9);
savepoint name3;
select * from savepointtest;
commit;

select 42;
select * from savepointtest;
drop table if exists savepointtest;