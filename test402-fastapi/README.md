[FastAPI][fastapi] + [SQLModel][sqlmodel] for quick backend creation.

Run server:

```console
$ poetry install
$ poetry run uvicorn main:app
...
INFO:     Uvicorn running on http://127.0.0.1:8000 (Press CTRL+C to quit)
```

Play with the API (using [httpie][httpie]):

```console
# POST an entry
$ echo '{"text": "The quick brown fox"}' | http :8000/notes/
HTTP/1.1 200 OK
...
{
    "id": 1,
    "text": "The quick brown fox"
}

# GET the added entry
$ http :8000/notes/1
HTTP/1.1 200 OK
...
{
    "id": 1,
    "text": "The quick brown fox"
}
```

[fastapi]: https://github.com/tiangolo/fastapi/
[sqlmodel]: https://github.com/tiangolo/sqlmodel/
[httpie]: https://httpie.io/
