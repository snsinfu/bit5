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

# Nomad from HashiCorp repository

curl -fsSL https://apt.releases.hashicorp.com/gpg | sudo apt-key add -
apt-add-repository "deb [arch=amd64] https://apt.releases.hashicorp.com $(lsb_release -cs) main"
sudo apt-get update
sudo apt-get install -y nomad

sudo systemctl enable nomad.service
sudo systemctl start nomad.service
