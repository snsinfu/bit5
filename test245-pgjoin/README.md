# Deno postgres

```console
$ docker-compose up --build
...
main_1      | QueryArrayResult {
main_1      |   query: Query {
main_1      |     text: "\n  SELECT board.id, owner.token_key\n  FROM board\n  JOIN owner ON board.owner = owner.id;\n",
main_1      |     args: []
main_1      |   },
main_1      |   _done: true,
main_1      |   warnings: [],
main_1      |   rows: [ [ "3e480486", "flQnXREky84028ig" ] ],
main_1      |   rowDescription: RowDescription {
main_1      |     columnCount: 2,
main_1      |     columns: [
main_1      |       Column {
main_1      |         name: "id",
main_1      |         tableOid: 16393,
main_1      |         index: 1,
main_1      |         typeOid: 25,
main_1      |         columnLength: -1,
main_1      |         typeModifier: -1,
main_1      |         format: 0
main_1      |       },
main_1      |       Column {
main_1      |         name: "token_key",
main_1      |         tableOid: 16385,
main_1      |         index: 2,
main_1      |         typeOid: 25,
main_1      |         columnLength: -1,
main_1      |         typeModifier: -1,
main_1      |         format: 0
main_1      |       }
main_1      |     ]
main_1      |   },
main_1      |   command: "SELECT",
main_1      |   rowCount: 1
main_1      | }
test243-pgdate_main_1 exited with code 0
test243-pgdate_main_1 exited with code 0

$ docker-compose down -v
```
