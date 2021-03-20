interface KV {
  key: string;
  value: string;
}

function objectToKV(obj?: object): KV[] {
  if (obj === undefined) {
    return [];
  }
  return Object.entries(obj).map(
    ([key, value]: [string, any]) => ({ key, value: value.toString() }),
  );
}

function queryToKV(query?: URLSearchParams): KV[] {
  if (query === undefined) {
    return [];
  }
  return Array.from(query.entries()).map(
    ([key, value]: [string, string]) => ({ key, value }),
  );
}

const params = {
  oauth_consumer_key: "xxxxxxxxxxxxxxxx",
  oauth_token: "yyyyyyyyyyyyyyyy",
  oauth_timestamp: 1600000000,
};

const query = new URLSearchParams();
query.append("page", "10");
query.append("page_size", "30");

const kv = [...objectToKV(params), ...queryToKV(query), ...queryToKV(undefined)];
console.log(kv);
