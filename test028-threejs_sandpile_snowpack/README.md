Install dependencies and start a dev server:

```console
$ yarn install
$ yarn snowpack dev
```

Then, open http://localhost:8080.

Or, you can `snowpack build` the app and serve it via a web server. The
following example uses Python's http.server module:

```console
$ yarn snowpack build
$ cd build
$ python3 -m http.server
Serving HTTP on 0.0.0.0 port 8000 (http://0.0.0.0:8000/) ...
```
