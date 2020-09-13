#!/usr/bin/python

DOCUMENTATION = """
module: s3put
options:
  region:
    type: str
    default: None
  bucket:
    type: str
  key:
    type: str
  content:
    type: str
    default: ""
  content_type:
    type: str
    default: "binary/octet-stream"
requirements:
  - boto3
  - botocore
"""

import os

import boto3

from ansible.module_utils.basic import AnsibleModule


def run(*, region, bucket, key, content, content_type):
    s3 = boto3.client("s3")
    response = s3.put_object(
        Bucket=bucket, Key=key, Body=content.encode("utf-8"), ContentType=content_type,
    )
    return dict(changed=True)


def main():
    module_args = dict(
        region=dict(type="str", required=False, default=None),
        bucket=dict(type="str", required=True),
        key=dict(type="str", required=True),
        content=dict(type="str", required=False, default=""),
        content_type=dict(type="str", required=False, default="binary/octet-stream"),
    )

    module = AnsibleModule(argument_spec=module_args, supports_check_mode=True)
    if module.check_mode:
        module.exit_json(changed=False)

    result = run(**module.params)
    module.exit_json(**result)


if __name__ == "__main__":
    main()
