from scapy.all import *


dests = [
    "10.0.0.1",
    "172.16.0.1",
    "192.168.0.1",
]

for dest in dests:
    print(dest, conf.route.route(dest))
