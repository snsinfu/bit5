import { createDbWorker } from "sql.js-httpvfs";

const workerUrl = new URL("sql.js-httpvfs/dist/sqlite.worker.js", import.meta.url);
const wasmUrl = new URL("sql.js-httpvfs/dist/sql-wasm.wasm", import.meta.url);

window.addEventListener("load", load, false);

async function load() {
  const worker = await createDbWorker(
    [
      {
        from: "inline",
        config: {
          serverMode: "full",
          url: "/data.sqlite3",
          requestChunkSize: 1024,
        },
      },
    ],
    workerUrl.toString(),
    wasmUrl.toString()
  );

  const input = document.getElementById("name") as HTMLInputElement;
  const button = document.getElementById("query") as HTMLButtonElement;
  const output = document.getElementById("output") as HTMLElement;
  const nameList = document.getElementById("names") as HTMLElement;

  (async () => {
    const results = await worker.db.exec("SELECT name FROM app_user");
    if (results.length === 1) {
      const {values: names} = results[0];
      nameList.textContent = "users: " + names.join(" ");
    }
  })();

  button.addEventListener("click", async () => {
    const name = input.value;

    const results = await worker.db.exec(
      "SELECT submission.url AS url"
      + " FROM submission"
      + " JOIN submitter, app_user"
      + " WHERE submitter.submission = submission.id"
      + " AND submitter.user = app_user.id"
      + " AND app_user.name = $name",
      {
        $name: name
      }
    );

    let message = "none";

    if (results.length === 1) {
      const {values: urls} = results[0];
      message = `${urls.length} urls:`;
      message += urls.join("\n");
      message += "\n";
    }

    output.textContent = message;
  });
}
