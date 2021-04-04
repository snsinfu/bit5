import { oak } from "./deps.ts";
import * as controller from "./web/controller.ts";
import * as database from "./database/mod.ts";
import * as myapp from "./myapp/mod.ts";

// CONFIGURATION -------------------------------------------------------------

function requireEnv(name: string): string {
  const value = Deno.env.get(name);
  if (!value) {
    throw new Error(`environment variable ${name} is required but not set`);
  }
  return value;
}

const config = {
  rootUrl: requireEnv("APP_URL"),
  gadgetlyKey: requireEnv("GADGETLY_KEY"),
  gadgetlySecret: requireEnv("GADGETLY_SECRET"),
};

const pool = new databases.Pool({
  host: requireEnv("POSTGRES_HOST"),
  port: parseInt(requireEnv("POSTGRES_PORT")),
  username: requireEnv("POSTGRES_USERNAME"),
  password: requireEnv("POSTGRES_PASSWORD"),
});

const main = new myapp.Main();

const app = new oak.Application<controller.State>({
  state: { config, pool, main },
});

// LOGGING -------------------------------------------------------------------

await log.setup({
  handlers: {
    console: new log.handlers.ConsoleHandler("DEBUG", {
      formatter: (record) => {
        let msg = "";
        msg += record.datetime.toISOString();
        msg += ` ${record.levelName} `;
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

router.get("/", controller.home);
router.get("/users", controller.users);

app.use(router.routes());
app.use(router.allowedMethods());

// ---------------------------------------------------------------------------

await app.listen({ hostname: "0.0.0.0", port: 3000 });
