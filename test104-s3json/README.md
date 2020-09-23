# Patch JSON document stored in S3 bucket

```console
$ poetry install
$ poetry run python main.py YOUR-BUCKET-NAME test104.json
! changed
{'my_project': {'metadata': {'date': '2020-09-23', 'some_id': 'S5mttlGEYk2XG4zz'}}}
$ poetry run python main.py YOUR-BUCKET-NAME test104.json
{'my_project': {'metadata': {'date': '2020-09-23', 'some_id': 'S5mttlGEYk2XG4zz'}}}
```

The operation is idempotent.
