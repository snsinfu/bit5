# Trello authorization

Set app credentials:

```console
$ export TRELLO_APP_KEY=...
$ export TRELLO_APP_SECRET=...
```

Run test service. Open the URL in your browser.

```console
$ ./run.sh
2021-03-28T16:17:38.537Z [INFO] Listening on http://localhost:3000
2021-03-28T16:17:44.772Z [INFO] 200 => GET / (from ::1)
```

Click "Connect to Trello". You will be redirected to an authorization page on
Trello. Click a button to allow test263 to access to Trello. Then, you will be
redirected back to the home page of the test service.

Next, click "Inspect database". You will see a JSON representation of in-memory
mock database containing user info and access key.
