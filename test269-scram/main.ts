import * as scram from "./scram.ts";

// Simulate a successful SCRAM authentication (example seen in RFC 5802).

const client = new scram.Client("user", "pencil", "fyko+d2lbbFgONRv9qkxdawL");

const c1 = client.composeChallenge();
const s1 = "r=fyko+d2lbbFgONRv9qkxdawL3rfcNHYJY1ZVvWVs7j,s=QSXCR+Q6sek8bf92,i=4096";
client.receiveChallenge(s1);
const c2 = client.composeResponse();
const s2 = "v=rmF9pqV8S7suAoZWja4dJRkFsKQ=";
client.receiveResponse(s2);

console.log("C:", c1);
console.log("S:", s1);
console.log("C:", c2);
console.log("S:", s2);
