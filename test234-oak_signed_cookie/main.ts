import * as oak from "deno.land/x/oak/mod.ts";

const app = new oak.Application({
    keys: ["secret key"]  // HMAC secret for signing cookies
});
const router = new oak.Router();

router.get("/", async (context: oak.Context) => {
    context.response.body = {
        message: "OK",
        error: null,
    };
    context.cookies.set("session", "171ec5c4592648ec6c9d46e825715e68", { signed: true, sameSite: "strict" });
});

app.use(async (context: oak.Context, next) => {
    try {
        await next();
    } catch (e) {
        if (e instanceof Error) {
            console.log("Unhandled Error:", e.message);
            console.log(e.stack);
        }
        throw e;
    }
});

app.use(router.routes());
app.use(router.allowedMethods());

await app.listen({ hostname: "127.0.0.1", port: 8080 });