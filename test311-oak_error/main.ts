import { oak } from "./deps.ts";

const app = new oak.Application();

const router = new oak.Router();

router.get("/", async (ctx: oak.Context) => {
  ctx.response.body = "OK\n";
});

router.get("/403", async (ctx: oak.Context) => {
  ctx.response.status = 403;
});

router.get("/500", async (ctx: oak.Context) => {
  ctx.response.status = 500;
});

app.use(async (ctx: oak.Context, next) => {
  await next();

  switch (ctx.response.status) {
    case 403:
      ctx.response.body = "You are not allowed to access this page.\n";
      break;

    case 404:
      ctx.response.body = "The requested page is not found.\n";
      break;

    case 500:
      ctx.response.body = "Something went wrong!\n";
      break;
  }
});

app.use(router.routes());
app.use(router.allowedMethods());

await app.listen({ hostname: "localhost", port: 3000 });
