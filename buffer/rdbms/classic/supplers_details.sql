-- http://www.postgresql.org/docs/9.1/static/datatype-enum.html

create type s_t as enum ('s1', 's2', 's3', 's4', 's5');
create type p_t as enum ('p1', 'p2', 'p3', 'p4', 'p5', 'p6');
--create type weight as int;
--create type color as text;

create table s (
  s s_t primary key,
  sname text not null,
  status int,
  city text not null
);

create table p(
  p p_t primary key,
  pname text not null,
  color text not null, --color,  TODO: сделать правильно
  weight real, --weight,
  city text not null
);

-- только после объявляния зависимы таблиц
create table sp (
  s s_t references s(s),
  p p_t references p(p),
  QTY int  -- Type QTY
);

-- сперва загружает основные таблыцы, и только потом отношения
COPY s FROM '/home/zaqwes/work/persistence-and-streams/rdbms/classic/supplers.csv' DELIMITER ',' CSV;
COPY p FROM '/home/zaqwes/work/persistence-and-streams/rdbms/classic/details.csv' DELIMITER ',' CSV;
COPY sp FROM '/home/zaqwes/work/persistence-and-streams/rdbms/classic/sp.csv' DELIMITER ',' CSV;


select * from s;
select * from p;
--select * from sp;


-- Порядок важен. Если удалить сперва типы, то таблыцы удалить будет нельзя
drop table s, sp, p;

drop type s_t, p_t; --, weight, color;
