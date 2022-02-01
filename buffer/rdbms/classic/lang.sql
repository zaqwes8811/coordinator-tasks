create table loadable (
  id int,
  key int,
  value int
);

-- try load data
-- http://stackoverflow.com/questions/2987433/how-to-import-csv-file-data-into-a-postgres-table
--
-- COPY loadable FROM 'data.csv' DELIMITER ',' CSV;  -- отсчет от  'SHOW data_directory;'
-- http://dba.stackexchange.com/questions/1350/how-do-i-find-postgresqls-data-directory
-- даже знак домашней директории не работает
COPY loadable FROM '/home/zaqwes/work/persistence-and-streams/rdbms/classic/data.csv' DELIMITER ',' CSV;

select * from loadable;

SHOW data_directory;

drop table loadable;