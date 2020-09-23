import argparse
import datetime
import json
import os
import signal

import boto3


def main():
    run(**parse_args())


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("bucket", metavar="<bucket>", type=str)
    parser.add_argument("key", metavar="<key>", type=str)
    return vars(parser.parse_args())


def run(*, bucket, key):
    data = fetch_json(bucket, key) or {}

    metadata = {
        "date": datetime.datetime.utcnow().strftime("%F"),
        "some_id": "S5mttlGEYk2XG4zz",
    }
    changed = patch(data, metadata, path=["my_project", "metadata"])

    if changed:
        put_json(bucket, key, data)
        print("! changed")

    print(data)


def patch(data, newdata, path):
    """
    Patch sub-structure of data at specified path with given new data. Missing
    intermediate hierarchy is created. Return True if data is changed.
    """
    for key in path[:-1]:
        if key not in data:
            data[key] = {}
        data = data[key]

    key = path[-1]
    if data.get(key) == newdata:
        return False

    data[key] = newdata
    return True


def fetch_json(bucket, key):
    """
    Fetch JSON-encoded object from S3 bucket.
    """
    try:
        s3 = boto3.resource("s3")
        obj = s3.Object(bucket, key)
        obj_body = obj.get()["Body"]
        data = json.load(obj_body)
    except s3.meta.client.exceptions.NoSuchKey:
        return None
    return data


def put_json(bucket, key, data):
    """
    Put JSON-encoded object to S3 bucket.
    """
    s3 = boto3.resource("s3")
    obj = s3.Object(bucket, key)
    obj.put(
        Body=json.dumps(data).encode("utf-8"), ContentType="application/json",
    )


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        signal.signal(signal.SIGINT, signal.SIG_DFL)
        os.kill(os.getpid(), signal.SIGINT)
