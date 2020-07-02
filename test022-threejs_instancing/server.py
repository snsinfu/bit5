#!/usr/bin/env python3

import http.server
import socketserver

# https://stackoverflow.com/a/15278302
socketserver.TCPServer.allow_reuse_address = True

Handler = http.server.SimpleHTTPRequestHandler
host = "localhost"
port = 4000

with socketserver.TCPServer((host, port), Handler) as httpd:
    print(f"Serving on http://{host}:{port}/")
    httpd.serve_forever()
