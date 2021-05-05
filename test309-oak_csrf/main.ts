import { oak } from "./deps.ts";
import * as routes from "./routes.ts";
import * as middleware from "./middleware.ts";

const app = new oak.Application();

const router = new oak.Router();
router.get("/", routes.home);
router.post("/update", routes.update);

app.use(middleware.csrfGuard());
app.use(router.routes());
app.use(router.allowedMethods());

await app.listen({ hostname: "localhost", port: 3000 });
