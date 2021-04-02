import * as postgres from "https://github.com/snsinfu/deno-postgres/raw/feat-scram/mod.ts";

const client = new postgres.Client({
  user: "test",
  password: "test",
  database: "test",
  hostname: "localhost",
  port: 5432,
});
await client.connect();

await client.queryObject`
CREATE TABLE IF NOT EXISTS card (
  id    TEXT PRIMARY KEY,
  label TEXT
);
`;
