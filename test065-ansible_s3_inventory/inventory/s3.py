#!/usr/bin/env python

import argparse
import json
import os

import boto3


def main(do_list, host):
    if do_list:
        run_list()
    if host:
        run_host(host)


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--list", dest="do_list", action="store_true", default=False)
    parser.add_argument("--host", type=str, default=None)
    return vars(parser.parse_args())


def run_list():
    inventory = fetch_inventory()
    hosts = inventory["all"]["hosts"]
    groups = inventory["all"]["children"]

    # https://docs.ansible.com/ansible/latest/dev_guide/developing_inventory.html#tuning-the-external-inventory-script
    output = {
        "_meta": {
            "hostvars": hosts,
        },
        "all": {
            "children": groups,
        }
    }
    return json.dumps(output)


def run_host(host):
    inventory = fetch_inventory()
    output = inventory["all"]["hosts"][host]
    return json.dumps(output)


def fetch_inventory():
    bucket = os.getenv("ANSIBLE_S3_INVENTORY_BUCKET")
    key = os.getenv("ANSIBLE_S3_INVENTORY_KEY")

    s3 = boto3.resource("s3")
    obj = s3.Object(bucket, key)
    inventory = json.load(obj.get()["Body"])

    return inventory


if __name__ == "__main__":
    main(**parse_args())
