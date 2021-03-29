// PostgreSQL startup messaging
//
// https://www.postgresql.org/docs/current/protocol-flow.html
// https://www.postgresql.org/docs/current/protocol-message-formats.html

const username = "main";
const database = "main";
const password = "28c9134e609f91e304307b569320a1f4";
const protocolVersion = 0x00030000;

async function main() {
  const conn = await Deno.connect({ hostname: "localhost", port: 5432 });
  try {
    await run(conn);
  } finally {
    conn.close();
  }
}

async function run(conn: Deno.Conn) {
  await startup(conn);
}

async function startup(conn: Deno.Conn) {
  await sendStartup(conn);
  await recvStartupResponse(conn);
}

async function sendStartup(conn: Deno.Conn) {
  const buf = new Deno.Buffer();

  write4(buf, 0);
  write4(buf, protocolVersion);

  writeString(buf, "user");
  write1(buf, 0);
  writeString(buf, username);
  write1(buf, 0);

  writeString(buf, "database");
  write1(buf, 0);
  writeString(buf, username);
  write1(buf, 0);

  write1(buf, 0); // Terminator

  const data = buf.bytes({ copy: false });
  const view = new DataView(data.buffer);
  view.setUint32(0, data.byteLength);

  await conn.write(data);
}

async function recvStartupResponse(conn: Deno.Conn) {
  const category = String.fromCharCode(await read1(conn));
  const size = await read4(conn);

  console.log("Response", { category, size });

  if (category === "E") {
    console.log("-> error response");
    return;
  }

  if (category === "R") {
    console.log("-> authentication request");

    const authType = await read4(conn);

    switch (authType) {
      case 0:
        console.log("-> AuthenticationOk");
        break;

      case 3:
        console.log("-> AuthenticationCleartextPassword");
        break;

      case 5:
        console.log("-> AuthenticationMD5Password");
        break;

      case 10:
        console.log("-> AuthenticationSASL");
        break;
    }
  }
}

/** Reads an unsigned 8-bit integer. */
async function read1(r: Deno.Reader): Promise<number> {
  const array = new Uint8Array(1);
  await r.read(array);
  return array[0];
}

/** Reads an unsigned 32-bit integer. */
async function read4(r: Deno.Reader): Promise<number> {
  const array = new Uint8Array(4);
  const view = new DataView(array.buffer);
  await r.read(array);
  return view.getUint32(0);
}

/** Writes an unsigned 8-bit integer. */
async function write1(w: Deno.Writer, val: number) {
  w.write(new Uint8Array([val]));
}

/** Writes an unsigned 32-bit integer. */
async function write4(w: Deno.Writer, val: number) {
  const array = new Uint8Array(4);
  const view = new DataView(array.buffer);
  view.setUint32(0, val);
  w.write(array);
}

/**
 * Writes a string. This function just writes UTF-8 bytes. No length is
 * prepended or no terminator is appended.
 */
async function writeString(w: Deno.Writer, str: string) {
  const utf8 = new TextEncoder();
  await w.write(utf8.encode(str));
}

await main();
