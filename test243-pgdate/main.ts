import { postgres } from "./deps.ts"

const client = new postgres.Client();
await client.connect();

// Can't use `user` as the table name. It is a reserved SQL keyword. I don't
// like the choice of word `owner` here, but I did not come up with a good
// name. `account` or `customer` do not sound good either.
await client.queryObject`
  CREATE TABLE owner (
    id            TEXT PRIMARY KEY,
    token_key     TEXT,
    token_secret  TEXT
  );
`;

await client.queryObject`
  CREATE TABLE board (
    id    TEXT PRIMARY KEY,
    owner TEXT REFERENCES owner(id)
  );
`;

await client.queryObject`
  CREATE INDEX IX_board_owner ON board(owner);
`;

const ownerId = "b57fbce6";
const tokenKey = "flQnXREky84028ig";
const tokenSecret = "nm3C567dGaxuUW59blKVUmJ4qDGhSDve";
await client.queryObject`
  INSERT INTO owner VALUES (${ownerId}, ${tokenKey}, ${tokenSecret});
`;

const boardId = "3e480486";
await client.queryObject`
  INSERT INTO Board VALUES (${boardId}, ${ownerId});
`;


const results = await client.queryArray`
  SELECT board.id, owner.token_key
  FROM board
  JOIN owner ON board.owner = owner.id;
`;
console.log(results);
