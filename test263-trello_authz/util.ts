export function requireEnv(name: string): string {
  const value = Deno.env.get(name);
  if (value === undefined) {
    throw new Error(`environment variable ${name} is required but not set`);
  }
  return value;
}
