import { assert, denodb } from "./deps.ts";
import * as models from "./models.ts";

try {
  await Deno.remove("_test.db");
} catch (e) {
  if (!(e instanceof Deno.errors.NotFound)) {
    throw e;
  }
}

const connector = new denodb.SQLite3Connector({ filepath: "_test.db" });
const database = new denodb.Database(connector);

database.link([models.User, models.Session, models.Token]);

try {
  await database.sync();

  // There are two users.
  await models.User.create([
    { id: "84326ef06a5f76ab", name: "alice" },
    { id: "06980cd2188f9412", name: "bob" },
  ]);

  // Alice has two sessions, Bob has one session.
  await models.Session.create([
    { id: "11e8ea66a3f923379687e8a4", userId: "84326ef06a5f76ab" },
    { id: "d5a4f734e88dd03b9fc4079f", userId: "84326ef06a5f76ab" },
    { id: "c34f766d6f68bdcbd3ba9fbf", userId: "06980cd2188f9412" },
  ]);

  // Each user has one token.
  await models.Token.create([
    { key: "2O4CPkbA", secret: "6FPYKrvEnu9zEzrd", userId: "84326ef06a5f76ab" },
    { key: "zBlW20R6", secret: "90IRL9aowAFhtEn3", userId: "06980cd2188f9412" },
  ]);

  console.log(([] as any[]).concat(
    await models.User.all(),
    await models.Session.all(),
    await models.Token.all(),
  ));

  // Alice deletes her account.
  const sessionId = "11e8ea66a3f923379687e8a4";
  const session = await models.Session.find(sessionId);
  if (session) {
    const userId = session.userId as string;
    await models.User.deleteById(userId);

    // All resources associated to the deleted user are cascade-deleted.
    assert(await models.Session.where("userId", "=", userId).count() === 0);
    assert(await models.Token.where("userId", "=", userId).count() === 0);
  }
} finally {
  await database.close();
}
