import { log, oak, oauth } from "./deps.ts";
import * as repository from "./repository.ts";
import * as routes from "./routes.ts";
import * as util from "./util.ts";

const TRELLO_APP_KEY = util.requireEnv("TRELLO_APP_KEY");
const TRELLO_APP_SECRET = util.requireEnv("TRELLO_APP_SECRET");
const TRELLO_URL = "https://trello.com/1";

const app = new oak.Application({
  state: {
    trello: new oauth.Api({
      consumer: { key: TRELLO_APP_KEY, secret: TRELLO_APP_SECRET },
      signature: oauth.HMAC_SHA1,
      prefix: TRELLO_URL,
    }),
    database: new repository.Database(),
  },
});

// LOGGING -------------------------------------------------------------------

await log.setup({
  handlers: {
    console: new log.handlers.ConsoleHandler("DEBUG", {
      formatter: (record) => {
        let msg = "";
        msg += record.datetime.toISOString();
        msg += " [";
        msg += record.levelName;
        msg += "] ";
        msg += record.msg;
        return msg;
      },
    }),
  },

  loggers: {
    default: {
      level: "INFO",
      handlers: ["console"],
    },
  },
});

// Log every request with response status.
app.use(async (context: oak.Context, next) => {
  const { ip, method, url } = context.request;
  const requestLog = `${method} ${url.pathname} (from ${ip})`;

  try {
    await next();

    const { status } = context.response;
    if (status >= 400) {
      log.warning(`${status} => ${requestLog}`);
    } else {
      log.info(`${status} => ${requestLog}`);
    }
  } catch (e) {
    log.error(`XXX => ${requestLog}`);
    if (e instanceof Error) {
      log.error(e.stack);
    }
    throw e;
  }
});

// Log actual socket used.
app.addEventListener("listen", (event) => {
  log.info(`Listening on http://${event.hostname}:${event.port}`);
});

// ROUTING -------------------------------------------------------------------

const router = new oak.Router();

router.get("/", routes.home);
router.get("/connect", routes.connect);
router.get("/connect/callback", routes.callback);
router.get("/database", routes.database);

app.use(router.routes());
app.use(router.allowedMethods());

// ---------------------------------------------------------------------------

await app.listen({ hostname: "localhost", port: 3000 });
