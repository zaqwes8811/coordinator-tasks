
-- BIN# -> BIN
create table if not exists cellar(
bin int primary key not null, 
wine text not null,
producer text not null,
year int not null,
bottles  int,
ready int not null
);

insert into cellar (bin, wine, producer, year, bottles, ready) values
(2,  'Chardonnay',     'Buena Vista',   2001, 1,  2003),
(3,  'Chardonnay',     'Geyser Peak',   2001, 5,  2003),
(6,  'Chardonnay',     'Simi',          2000, 4,  2002),
(12, 'Joh. Riesling',  'Jekel',         2002, 1,  2003),
(21, 'Fume Blanc',     'Ch. St. Jean',  2001, 4,  2003),
(22, 'Fume Blanc',     'Robt. Mondavi', 2000, 2,  2002),
(30, 'Gewurztraminer', 'Ch. St. Jean',  2002, 3,  2003),
(43, 'Cab. Sauvignon', 'Windsor',       1995, 12, 2004),
(45, 'Cab. Sauvignon', 'Geyser Peak',   1998, 12, 2006),
(48, 'Cab. Sauvignon', 'Robt. Mondavi', 1997, 12, 2008),
(50, 'Pinot Noir',     'Gary Farrell',  2000, 3,  2003),
(51, 'Pinot Noir',     'Fetzer',        1997, 3,  2004),
(52, 'Pinot Noir',     'Dehlinger',     1999, 2,  2002),
(58, 'Merlot',         'Clos du Bois',  1998, 9,  2004),
(64, 'Zinfander',      'Cline',         1998, 9,  2007),
(72, 'Zinfander',      'Rafanelli',     1999, 2,  2007);


select wine, bin, producer 
  from cellar
  where ready = 2004;

drop table cellar;