import { assert } from "https://deno.land/std@0.89.0/testing/asserts.ts";
import * as model from "./model.ts";
import * as repository from "./repository.ts";

const users = new repository.Users();
const boards = new repository.Boards();
const cards = new repository.Cards();
const reapers = new repository.Reapers();
const sessions = new repository.Sessions();

// Login ---------------------------------------------------------------------

// User token obtained through OAuth.
users.upsert({
  id: "U/721c469a30a02f0a",
  name: "alice",
  tokenKey: "T/6f9971904403ed61",
  tokenSecret: "42f8c10359c172e70c0d17b1781e308b",
});

// Start a session.
sessions.upsert({
  id: "S/100a2704e6ee2852",
  user: "U/721c469a30a02f0a",
});

// Sync ----------------------------------------------------------------------

// User pass in a cookie with session ID.
const cookie = {
  session: "S/100a2704e6ee2852",
};

const session = sessions.findById(cookie.session);
assert(session !== undefined);

// Suppose that we fetch boards owned by the user here.
const { tokenKey, tokenSecret } = users.findById(session.user)!;

boards.upsert({
  id: "B/b4eb5c9a8fd21c89",
  name: "Personal Tasks",
  user: "U/721c469a30a02f0a",
});

boards.upsert({
  id: "B/447a9ed0494312a5",
  name: "Work",
  user: "U/721c469a30a02f0a",
});

// User clicked "enable" button on one of the boards. This sets up a Webhook
// on the connected service, and the webhook ID is stored.
reapers.upsert({
  id: "R/58d0dadaba54b73f",
  board: "B/b4eb5c9a8fd21c89",
  webhook: "W/042a1b66e83cd184",
  timeToLive: 3600,
});

// Notification --------------------------------------------------------------

// Webhook notification of card updates.
cards.upsert({
  id: "C/9695e9f2d3cc94d7",
  board: "B/b4eb5c9a8fd21c89",
  updateTime: 1615024119,
});

cards.upsert({
  id: "C/449e8e47d614273f",
  board: "B/b4eb5c9a8fd21c89",
  updateTime: 1615025265,
});

cards.upsert({
  id: "C/2ba019d6e759a8f4",
  board: "B/b4eb5c9a8fd21c89",
  updateTime: 1615028810,
});

// Periodic archive ----------------------------------------------------------

const now = 1615029344;

for (const reaper of reapers.all()) {
  const board = boards.findById(reaper.board)!;
  const user = users.findById(board.user)!;
  const targetCards = cards.findByBoard(board.id);

  for (const card of targetCards) {
    if (now - card.updateTime >= reaper.timeToLive) {
      // Call API of the connected service to archive stale card.
      console.log("Archive card:", card.id);
      console.log("  Token:", user.tokenKey);
    }
  }
}
