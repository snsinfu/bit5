function merge<A extends object, B extends object>(a: A, b: B): A & B {
  return Object.assign({}, a, b);
}

const a = {
  token: "33e48d0c731a62e3",
  data: 0,
};

const b = {
  callback: "https://example.com/callback",
  data: "data",
};

// merge produces c.data = "data" (string), but the return type A & B does not
// correctly represent the situation and allows assignment to data: number
// without complaint.
const c: { token: string; callback: string; data: number; } = merge(a, b);

// This is type error.
//c.data = "data";

console.log(c);

// See: https://stackoverflow.com/questions/49682569/typescript-merge-object-types
