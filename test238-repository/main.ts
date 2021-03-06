import { assert } from "https://deno.land/std@0.89.0/testing/asserts.ts";
import * as model from "./model.ts";

const users = new model.UserRepository();

users.upsert({
    id: "2d0d1af395950f23",
    tokenKey: "zDQkfPuOU6bHOzUG",
    tokenSecret: "Tf63WbXLvNQahso2YLnulciTVWAcdZQ3",
});

users.upsert({
    id: "e05741d795421e1f",
    tokenKey: "VT8xcmYMuHS4PsjP",
    tokenSecret: "XZVv3m7R2VhzU4sfYNQDFPfDYKxveVG1",
});

// Update user token.
{
    const u = users.findByToken("zDQkfPuOU6bHOzUG")!;
    u.tokenKey = "glwsvoep8HiUBpNc";
    u.tokenSecret = "bBiEqMW11I1pmPiiphoRtRcGW7wtuAjQ";
    users.upsert(u);
}

// Inspect a user.
{
    const u = users.findById("2d0d1af395950f23");
    assert(u !== undefined);
    assert(u.tokenKey === "glwsvoep8HiUBpNc");
    console.log(u);
}

// Delete a user that has specific token.
{
    const u = users.findByToken("glwsvoep8HiUBpNc")!;
    assert(u.id === "2d0d1af395950f23");
    users.delete(u);
}

// Now, the user does not exist.
{
    const u = users.findById("2d0d1af395950f23");
    assert(u === undefined);
}

// Aonther user.
{
    const u = users.findById("e05741d795421e1f");
    console.log(u);
}
