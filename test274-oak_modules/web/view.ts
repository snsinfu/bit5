import { path, oak } from "../deps.ts";

export async function home(context: oak.Context) {
  await oak.send(context, "index.html", { root: "static" });
}

export interface UsersData {
  userList: User[];
}

export interface User {
}

export async function users(context: oak.Context, data: UsersData) {
}
