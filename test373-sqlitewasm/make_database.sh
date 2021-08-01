#!/bin/sh

rm -f data.sqlite3

sqlite3 data.sqlite3 <<END

-- Prepare dataset

CREATE TABLE app_user (
  id   INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT
);

CREATE TABLE submission (
  id  INTEGER PRIMARY KEY AUTOINCREMENT,
  url TEXT
);

CREATE TABLE submitter (
  submission INTEGER REFERENCES submission(id),
  user       INTEGER REFERENCES app_user(id)
);

INSERT INTO app_user (name) VALUES ('alice'), ('bob'), ('charlie');

INSERT INTO submission (url) VALUES ('http://example.com/blog');
INSERT INTO submitter SELECT last_insert_rowid(), id AS user_id FROM app_user WHERE name = 'alice';

INSERT INTO submission (url) VALUES ('http://example.com/about');
INSERT INTO submitter SELECT last_insert_rowid(), id AS user_id FROM app_user WHERE name = 'alice';

INSERT INTO submission (url) VALUES ('http://example.com/news');
INSERT INTO submitter SELECT last_insert_rowid(), id AS user_id FROM app_user WHERE name = 'bob';


-- Optimizations recommended by the httpvfs author

pragma journal_mode = delete;
pragma page_size = 1024;
vacuum;

END
