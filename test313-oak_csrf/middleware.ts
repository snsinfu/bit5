import { base64, oak } from "./deps.ts";

const COOKIE_KEY = "csrf_token";
const HEADER_KEY = "X-CSRF-Token";
const TOKEN_SIZE = 15;
const TOKEN_AGE = 86400;
const SAFE_METHODS = ["GET", "HEAD", "OPTIONS", "TRACE"];

export function csrfGuard(): oak.Middleware {
  return async (ctx: oak.Context, next) => {
    const { request, response, cookies } = ctx;
    const method = request.method;
    const realToken = cookies.get(COOKIE_KEY);
    const clientToken = request.headers.get(HEADER_KEY);
    let newToken = realToken;
    const sameOrigin = checkSameOrigin(request);

    if (!SAFE_METHODS.includes(method)) {
      if (!sameOrigin) {
        response.status = 403;
        return;
      }

      if (!realToken || !clientToken || !verifyToken(realToken, clientToken)) {
        response.status = 400;
        return;
      }

      newToken = undefined;
    }

    if (!newToken) {
      newToken = generateToken(TOKEN_SIZE);
    }

    // Do not leak token to other sites.
    if (sameOrigin) {
      cookies.set(COOKIE_KEY, newToken, { sameSite: "strict", maxAge: TOKEN_AGE });
      response.headers.set(HEADER_KEY, newToken);
    }

    await next();
  };
}

function checkSameOrigin(request: oak.Request): boolean {
  const referrer = request.headers.get("referer");
  if (!referrer) {
    return false;
  }
  return verifyOrigin(request.url, new URL(referrer));
}

function verifyOrigin(page: URL, referrer: URL): boolean {
  return page.protocol === referrer.protocol && page.host === referrer.host;
}

function verifyToken(session: string, client: string): boolean {
  return session === client;
}

function generateToken(size: number): string {
  return base64.encode(crypto.getRandomValues(new Uint8Array(size)));
}
