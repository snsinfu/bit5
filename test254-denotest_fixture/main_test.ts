import { assertEquals } from "https://deno.land/std@0.90.0/testing/asserts.ts";

// Can a global be used as a fixture?
let state = 0;

Deno.test("case 1", () => {
  state++;
  assertEquals(state, 1);
});

Deno.test("case 2", () => {
  state++;
  assertEquals(state, 1); // No. This fails.
});
