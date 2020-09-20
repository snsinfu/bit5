# https://duckdb.org/docs/api/python

import duckdb

con = duckdb.connect(database=":memory:", read_only=False)

con.execute("CREATE TABLE items(item VARCHAR, value DECIMAL(10,2), count INTEGER)")

row = ["laptop", 2000, 1]
con.execute("INSERT INTO items VALUES (?, ?, ?)", row)

rows = [["chainsaw", 500, 10], ["iphone", 300, 2]]
con.executemany("INSERT INTO items VALUES (?, ?, ?)", rows)

con.execute("SELECT item FROM items WHERE value > ?", [400])
print(con.fetchall())
