import { postgres } from "./deps.ts"

const client = new postgres.Client();
await client.connect();

await client.queryObject`
  CREATE TABLE customer (
    id   TEXT PRIMARY KEY,
    name TEXT
  );
  CREATE TABLE access_token (
    key    TEXT PRIMARY KEY,
    secret TEXT NOT NULL,
    owner  TEXT NOT NULL REFERENCES customer(id)
  );
  CREATE TABLE board (
    id    TEXT PRIMARY KEY,
    name  TEXT,
    owner TEXT NOT NULL REFERENCES customer(id)
  );
  CREATE TABLE card (
    id    TEXT PRIMARY KEY,
    board TEXT NOT NULL REFERENCES board(id)
  );
`;

const customer = {
  id: "8d73c94e7312251d99a1ede60ce26377", name: "alice"
};
await client.queryObject`
  INSERT INTO customer VALUES (${customer.id}, ${customer.name})
`;

const access_token = {
  key: "386ca19c4c1f88aa",
  secret: "73915ae20137f2557c47be4170badfd2",
  owner: customer.id,
};
await client.queryObject`
  INSERT INTO access_token VALUES (${access_token.key}, ${access_token.secret}, ${access_token.owner})
`;

const board = {
  id: "8d1e08d526038067c2994deaef459962", name: "Work", owner: customer.id
};
await client.queryObject`
  INSERT INTO board VALUES (${board.id}, ${board.name}, ${board.owner})
`;

const card = {
  id: "1ba344bbcd3366ac417059580a5a59c8", owner: board.id
};
await client.queryObject`
  INSERT INTO card VALUES (${card.id}, ${card.owner})
`;

const ownedCards = await client.queryObject`
  SELECT card.id AS card, board.owner
  FROM card
  JOIN board ON card.board = board.id
`;

for (const {card, owner} of ownedCards.rows) {
  console.log(`Card: ${card}`);

  // Should cache this query.
  const tokenQuery = await client.queryObject`
    SELECT key, secret FROM access_token WHERE access_token.owner = ${owner};
  `;
  if (tokenQuery.rows.length === 0) {
    console.log(`  No token for customer: ${owner}`);
    continue;
  }

  const {key, secret} = tokenQuery.rows[0]
  console.log(`  Use token: ${key}`);
  console.log(`  Use secret: ${secret}`);
}
