#!/bin/sh -eu

config="example.yaml"

# https://github.com/mikefarah/yq

namespace="$(yq e '.namespace' "${config}")"
deployments="$(yq e '.deployments[].name' "${config}")"
containers="$(yq e '.deployments[].containers[]' "${config}" -jI0)"

echo "--- Images to build ---"
echo "${containers}" | while read cont; do
    image="$(echo "${cont}" | yq e '.image' -)"
    build="$(echo "${cont}" | yq e '.build' -)"
    echo "${image} | ${build}"
done

echo "--- Deployments to restart ---"
echo "${deployments}" | while read deploy; do
    echo "${namespace}.${deploy}"
done
