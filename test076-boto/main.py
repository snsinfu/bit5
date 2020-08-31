#!/usr/bin/env python3

import argparse
import datetime
import json

import boto3


def main(*, bucket, key):
    s3 = boto3.resource("s3")
    obj = s3.Object(bucket, key)

    # Get
    try:
        data = json.load(obj.get()["Body"])
        print("Get:", data)
    except s3.meta.client.exceptions.NoSuchKey:
        data = {}
        print("Get: (none)")

    # Update
    data["date"] = datetime.datetime.utcnow().strftime("%F %T")

    # Put
    body = json.dumps(data).encode("utf-8")
    obj.put(Body=body)

    print("Put done.")


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("bucket", type=str)
    parser.add_argument("key", type=str, nargs="?", default="default.json")
    return vars(parser.parse_args())


if __name__ == "__main__":
    main(**parse_args())
