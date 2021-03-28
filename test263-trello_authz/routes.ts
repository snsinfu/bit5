import { oak, oauth } from "./deps.ts";
import * as repository from "./repository.ts";

// GET /
export async function home(context: oak.Context) {
  await oak.send(context, "index.html", { root: "static" });
}

// GET /connect
export async function connect(context: oak.Context) {
  const database = context.state.database as repository.Database;
  const trello = context.state.trello as oauth.Api;
  const tempTokens = new repository.TemporaryTokenStore(database);

  // Request a temporary token.
  const response = await trello.request("POST", "/OAuthGetRequestToken", {
    params: { oauth_callback: "http://localhost:3000/connect/callback" },
  });
  if (!response.ok) {
    console.log("failed to request temporary token");
    context.throw(oak.Status.InternalServerError);
  }
  const responseData = new URLSearchParams(await response.text());

  const tempKey = responseData.get("oauth_token");
  const tempSecret = responseData.get("oauth_token_secret");
  if (!tempKey || !tempSecret) {
    console.log("temporary token is not returned from trello");
    context.throw(oak.Status.InternalServerError);
  }

  tempTokens.insert({
    key: tempKey,
    secret: tempSecret,
    createTime: Date.now(),
  });

  // Redirect user to Trello's authorization page.
  const url = new URL(trello.prefix + "/OAuthAuthorizeToken");

  url.searchParams.append("oauth_token", tempKey);
  url.searchParams.append("expiration", "1hour");
  url.searchParams.append("scope", "read");
  url.searchParams.append("name", "test263");

  context.response.redirect(url);
}

// GET /connect/callback
export async function callback(context: oak.Context) {
  const database = context.state.database as repository.Database;
  const trello = context.state.trello as oauth.Api;

  const tempTokens = new repository.TemporaryTokenStore(database);
  const accessTokens = new repository.AccessTokenStore(database);
  const users = new repository.UserStore(database);

  // Get a temporary token and a verifier.
  const params = context.request.url.searchParams;
  const tempKey = params.get("oauth_token");
  const verifier = params.get("oauth_verifier");

  if (!tempKey || !verifier) {
    context.throw(oak.Status.BadRequest);
  }

  const tempSecret = tempTokens.findByKey(tempKey)?.secret;
  if (!tempSecret) {
    context.throw(oak.Status.BadRequest);
  }

  // Request an access token on behalf of the user.
  const response = await trello.request("POST", "/OAuthGetAccessToken", {
    token: { key: tempKey, secret: tempSecret },
    params: { oauth_verifier: verifier },
  });
  const responseData = new URLSearchParams(await response.text());
  const tokenKey = responseData.get("oauth_token");
  const tokenSecret = responseData.get("oauth_token_secret");

  if (!tokenKey || !tokenSecret) {
    context.throw(oak.Status.InternalServerError);
  }
  const token = { key: tokenKey, secret: tokenSecret };

  tempTokens.deleteByKey(tempKey);

  // Get user information.
  const meResponse = await trello.request("GET", "/members/me", { token });
  const meData = await meResponse.json();
  const id = meData.id;
  const username = meData.username;

  users.upsert({ id: id, name: username });
  accessTokens.insert({ ...token, userId: id });

  context.response.redirect("/");
}

// GET /database
export async function database(context: oak.Context) {
  context.response.type = "json";
  context.response.body = context.state.database as repository.Database;
}
