/** Throws an error in an expression. */
function error(message: string): never {
  throw new Error(message);
}

interface Token {
  key: string;
  secret: string;
}

// "undefined" is stripped out, so rhs is assignable to Token type.
const appToken: Token = {
  key: Deno.env.get("APP_KEY") || error("APP_KEY is required"),
  secret: Deno.env.get("APP_SECRET") || error("APP_SECRET is required"),
};

console.log(appToken);
