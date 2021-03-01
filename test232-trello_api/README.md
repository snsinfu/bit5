# Trello API

Log in to Trello and go to [the API key page][key]. Take note of your API key
shown in the page. Then, fill the `key=` parameter in the URL below and open
the URL in the browser:

```
https://trello.com/1/authorize?expiration=1day&name=TestToken&scope=read&response_type=token&key=
```

The page will show an API token. The token is short-lived (expires in 1 day)
and only have `read` scope. Now, set API key and API token to environment
variables:

```console
$ cat > .env
TRELLO_API_KEY=...
TRELLO_API_TOKEN=...
$ dotenv
dotenv: Loading .env
```

And, run the `main.ts` script to retrieve your account information:

```console
$ deno run --allow-net --allow-env main.ts
{
  id: "5b...",
  ...
  email: null,
  ...
}
```

- The account ID would be useful for managing users in an external Trello
  application. The account ID will be necessary to renew API token.
- As described in [the API documentation][doc], the `email` field is deducted.
  Nice.

[key]: https://trello.com/app-key
[doc]: https://developer.atlassian.com/cloud/trello/guides/rest-api/authorization
