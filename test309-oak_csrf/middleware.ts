import { base64, oak } from "./deps.ts";

const DEFAULT_COOKIE_KEY = "csrf_token";
const DEFAULT_HEADER_KEY = "X-CSRF-Token";
const DEFAULT_TOKEN_SIZE = 15;
const DEFAULT_TOKEN_AGE = 86400;

// CSRF protection. Note that this middleware uses a cookie just as a
// persistent storage for a CSRF token. The cookie is not used for verifying if
// a request is legid. Instead, this middleware checks if a valid CSRF token is
// passed in a custom HTTP header. One can also use a database as a storage for
// the CSRF token, but it requires a session to be established.

export interface CsrfOptions {
  cookie?: string;
  header?: string;
}

export function csrfGuard(opts?: CsrfOptions): oak.Middleware {
  const headerKey = opts?.header ?? DEFAULT_HEADER_KEY;
  const cookieKey = opts?.cookie ?? DEFAULT_COOKIE_KEY;

  return async (ctx: oak.Context, next) => {
    const token = ctx.cookies.get(cookieKey);
    const method = ctx.request.method;

    if (isProtectedMethod(method)) {
      const header = ctx.request.headers.get(headerKey);

      if (!header || header !== token) {
        ctx.response.status = oak.Status.BadRequest;
        return;
      }
    }

    const newToken = isProtectedMethod(method) || !token
      ? generateToken(DEFAULT_TOKEN_SIZE)
      : token;
    ctx.cookies.set(
      cookieKey,
      newToken,
      { maxAge: DEFAULT_TOKEN_AGE, sameSite: "strict" },
    );
    ctx.state.csrfToken = token;

    await next();
  };
}

function isProtectedMethod(method: oak.HTTPMethods): boolean {
  switch (method) {
    case "GET":
    case "HEAD":
    case "OPTIONS":
      return false;
    default:
      return true;
  }
}

function generateToken(size: number): string {
  return base64.encode(crypto.getRandomValues(new Uint8Array(size)));
}
