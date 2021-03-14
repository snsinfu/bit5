function must<T>(value?: T, message?: string): T {
  if (value === undefined) {
    throw new Error(message || "unexpected undefined value");
  }
  return value;
}

const appToken = {
  key: must(Deno.env.get("APP_KEY"), "APP_KEY is required"),
  secret: must(Deno.env.get("APP_SECRET"), "APP_SECRET is required"),
};
console.log(appToken);
