#!/bin/sh -eux

export DEBIAN_FRONTEND=noninteractive

# Packages for adding non-official repositories

sudo apt-get update
sudo apt-get install -y \
    curl \
    apt-transport-https \
    ca-certificates \
    gnupg-agent \
    software-properties-common

# Nomad/Consul from HashiCorp repository

curl -fsSL https://apt.releases.hashicorp.com/gpg | sudo apt-key add -
apt-add-repository "deb [arch=amd64] https://apt.releases.hashicorp.com $(lsb_release -cs) main"
sudo apt-get update
sudo apt-get install -y nomad consul

sudo tee /etc/nomad.d/consul.hcl << EOF
consul {
    address = "127.0.0.1:8500"
}
EOF
sudo systemctl enable nomad.service
sudo systemctl start nomad.service

sudo tee /etc/consul.d/consul-local.hcl << EOF
bind_addr = "127.0.0.1"
server = true
bootstrap_expect = 1
EOF
sudo systemctl enable consul.service
sudo systemctl start consul.service

# Docker

curl -fsSL https://download.docker.com/linux/debian/gpg | sudo apt-key add -
apt-add-repository "deb [arch=amd64] https://download.docker.com/linux/debian $(lsb_release -cs) stable"
sudo apt-get update
sudo apt-get install -y docker-ce docker-ce-cli containerd.io

sudo systemctl enable docker.service
sudo systemctl start docker.service
