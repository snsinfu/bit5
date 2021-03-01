const API_KEY = Deno.env.get("TRELLO_API_KEY");
const API_TOKEN = Deno.env.get("TRELLO_API_TOKEN");
const API_BASE = "https://api.trello.com/1";

if (API_KEY === undefined || API_TOKEN === undefined) {
    console.error("Specify API key and token")
    Deno.exit(1);
}

const resource = "/members/me";
const query = new URLSearchParams();
query.append("key", API_KEY);
query.append("token", API_TOKEN);

const response = await fetch(API_BASE + resource + "?" + query.toString());
console.log(await response.json());
