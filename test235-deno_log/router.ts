import { oak } from "./deps.ts";

export const router = new oak.Router();

router.get("/", async (context: oak.Context) => {
  context.response.body = {
    message: "OK",
  };
});

router.get("/crash", async (context: oak.Context) => {
  throw new Error("crash");
});
