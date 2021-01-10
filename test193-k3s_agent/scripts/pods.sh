#!/bin/sh

columns="NAME:.metadata.name,NODE:.spec.nodeName,IP:.status.podIP"

kubectl get pods -o custom-columns="${columns}" "$@"
