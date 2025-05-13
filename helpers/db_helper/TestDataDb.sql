-- ------------------------------------------------------ --
--                 SQlite3 Database                       --
-- Contains data for the count sorting algorithm          --
-- ------------------------------------------------------ --

DROP TABLE IF EXISTS Strings;
DROP TABLE IF EXISTS Letters;
DROP TABLE IF EXISTS ResultPassword;

CREATE TABLE Strings (
  string_id INTEGER PRIMARY KEY AUTOINCREMENT,
  base TEXT NOT NULL,
  sorted  TEXT NOT NULL
);

CREATE TABLE Letters (
  letter_id INTEGER PRIMARY KEY AUTOINCREMENT,
  letter TEXT NOT NULL,
  letter_frequency INTEGER
);
--CREATE INDEX letter_hash ON Letters(letter);

CREATE TABLE ResultPassword (
  password_id INTEGER PRIMARY KEY AUTOINCREMENT,
  result TEXT NOT NULL
);


INSERT INTO Strings (base, sorted) VALUES ('hzt', 'cqc'), ('sng', 'ena'), ('ena', 'hpo'), ('sdt', 'hzt'),
										  ('qds', 'lpz'), ('yif', 'qds'), ('slt', 'sdt'), ('lpz', 'slt'),
										  ('cqc', 'sng'), ('hpo', 'yif');

INSERT INTO Letters (letter, letter_frequency) VALUES ('a', 1), ('b', 0), ('c', 2), ('d', 2), ('e', 1), ('f', 1),
													  ('g', 1), ('h', 2), ('i', 1), ('j', 0), ('k', 0), ('l', 2),
													  ('m', 0), ('n', 2), ('o', 1), ('p', 2), ('q', 2), ('r', 0),
													  ('s', 4), ('t', 3), ('u', 0), ('v', 0), ('w', 0), ('x', 0),
													  ('y', 1), ('z', 2);


INSERT INTO ResultPassword (result) VALUES ('cqcsyif');
