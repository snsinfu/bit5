import * as scram from "./scram.ts";

const client = new scram.ProtocolClient("alice", "passw0rd", "nonce1");
const c1 = client.makeInitialMessage();
const s1 = "r=nonce1nonce2,s=c2FsdA==,i=10000";
const c2 = client.makeFinalMessage(s1);
const s2 = "v="; // server signature?

console.log("C:", c1);
console.log("S:", s1);
console.log("C:", c2);
console.log("S:", s2);

