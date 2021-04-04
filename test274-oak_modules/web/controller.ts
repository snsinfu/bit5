import { oak } from "./deps.ts";
import * as myapp from "../myapp/mod.ts";
import * as view from "./view.ts";

export interface Config {
  rootUrl: string;
  externalApiKey: string;
}

export interface State {
  config: Config;
  pool: databases.Pool;
  main: myapp.Main;
}

type Context = oak.Context<State>;

export async function home(context: Context) {
  await view.home(context);
}

export async function users(context: Context) {
  const { pool, main } = context.state;

  const conn = pool.connect();
  try {
    await view.users(context, { userList: main.listUsers(conn) });
  } catch (e) {
    conn.close();
    throw e;
  }
}
