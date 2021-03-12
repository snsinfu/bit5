#!/bin/sh -eu

cd "${HOME}"

k3s_version="v1.20.4+k3s1"
kim_version="v0.1.0-alpha.3"

export DEBIAN_FRONTEND=noninteractive
apt-get update -qq
apt-get install -qq -y --no-install-recommends curl

mkdir -p /etc/rancher/k3s
cat > /etc/rancher/k3s/config.yaml << END
disable: traefik
write-kubeconfig-mode: "644"
END

curl -fsSLo k3s_install.sh "https://github.com/k3s-io/k3s/raw/${k3s_version}/install.sh"
bash k3s_install.sh

curl -fsSLo kim "https://github.com/rancher/kim/releases/download/${kim_version}/kim-linux-amd64"
install -m 755 kim /usr/local/bin/kim
mkdir -p /var/lib/buildkit

kim builder install
