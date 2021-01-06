#!/bin/sh -eu

reponame="$(basename "${PWD}")"
registry="registry:5000"

buildroot="${HOME}/build/${reponame}"

cleanup() {
    rm -rf "${buildroot}"
}
trap cleanup EXIT

while read oldrev newrev refname; do
    echo "Creating build directory"
    mkdir -p "${buildroot}"

    branch="${refname#refs/heads/}"
    echo "Checking out ${branch}:${newrev}"
    git archive "${newrev}" | tar -x -C "${buildroot}"

    imagename="${reponame}"
    imagespec="${registry}/${imagename}:${newrev}"
    echo "Building image"
    docker build -t "${imagespec}" "${buildroot}"

    echo "Pushing image"
    docker push "${imagespec}"

    manifests="${buildroot}/manifests"
    echo "Applying manifests"
    kubectl apply -f "${manifests}"

    echo "Cleaning up build directory"
    rm -rf "${buildroot}"
done
