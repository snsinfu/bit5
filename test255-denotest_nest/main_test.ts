import { assertExists } from "https://deno.land/std@0.90.0/testing/asserts.ts";

Deno.test("object", () => {
  const thing = {
    name: "whatchamacallit",
  };

  // This test does not run. Because Deno.test just registers a test function
  // to the list of tests at runtime. This test is indeed added to the list but
  // the test driver is already running tests in the old list of tests, so this
  // new test won't run.
  Deno.test("has name", () => {
    assertExists(thing.name);
  });
});
