import * as denodb from "https://deno.land/x/denodb@v1.0.31/mod.ts";

const connector = new denodb.SQLite3Connector({ filepath: ":memory:" });
const database = new denodb.Database(connector);

class Token extends denodb.Model {
  static table = "tokens";
  static fields = {
    key: { type: denodb.DataTypes.TEXT, primaryKey: true },
    secret: denodb.DataTypes.TEXT,
  };
}

database.link([Token]);

try {
  await database.sync();

  await Token.create([
    { key: "af3eb69222e74989", secret: "qmqkxLlrmnkIM2Es" },
    { key: "b29472fe67bd8764", secret: "fzRZVrc9vdIP1Cf7" },
  ]);

  // XXX: find() is not typed to return an undefined but it silently returns
  // one when no row is found. This line should be a type error!
  const token: Token = await Token.find("none");

  // XXX: denoDB model feilds are not statically typed! This line causes error
  // Type 'Function | FieldValue' is not assignable to type 'string'. This is
  // really bad as I would have to put type assertions everywhere in my code.
  // Am I missing something?
  const key: string = token.key;

} finally {
  await database.close();
}
