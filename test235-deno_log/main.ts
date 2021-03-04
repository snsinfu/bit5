import { log, oak } from "./deps.ts";
import { router } from "./router.ts";

const app = new oak.Application();

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
  log.info(`Listening on ${event.hostname}:${event.port}`);
});

// ROUTER --------------------------------------------------------------------

app.use(router.routes());
app.use(router.allowedMethods());

await app.listen({ hostname: "localhost", port: 8080 });
