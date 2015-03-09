-- Design:
--   Task can have [0, N) tags
--   Tag can have [0, M) tasks
--   Tags names is unique!
--
-- Looks like ref. integrity not needed
--   http://www.postgresql.org/docs/8.0/static/tutorial-fk.html

-- Test ideas

CREATE TABLE if not exists weather (
    city            varchar(80),
    temp_lo         int,           -- low temperature
    temp_hi         int,           -- high temperature
    prcp            real,          -- precipitation
    date            date
);

CREATE TABLE if not exists cities (
    name            varchar(80),
    location        point
);

INSERT INTO weather VALUES ('San Francisco', 46, 50, 0.25, '1994-11-27');
INSERT INTO cities VALUES ('San Francisco', '(-194.0, 53.0)');

INSERT INTO weather (city, temp_lo, temp_hi, prcp, date)  VALUES ('San Francisco', 43, 57, 0.0, '1994-11-29');

INSERT INTO weather (date, city, temp_hi, temp_lo)  VALUES ('1994-11-29', 'Hayward', 54, 37);

--select * from weather;
--select * from cities;

-- Joins
SELECT *
    FROM weather, cities
    WHERE city = name;

-- Many-to-many

drop table cities;
drop table weather;