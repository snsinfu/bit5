# DuckDB

Looks interesting. No Go bindings?

- https://duckdb.org/
- https://news.ycombinator.com/item?id=24531085

To run:

```console
$ poetry install
$ poetry run python main.py
[('laptop',), ('chainsaw',)]
$ rm -r $(poetry env info -p)
```

The installation takes a fair amount of time (building some C++ libraries it
seems). Also somehow `duckdb` depends on numpy.
