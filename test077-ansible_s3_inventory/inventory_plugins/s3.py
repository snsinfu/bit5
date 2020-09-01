DOCUMENTATION = """
    name: s3
    plugin_type: s3
    options:
        plugin:
            required: yes
            choices: ["s3"]
        s3_url:
            description: S3 URL endpoint.
            type: str
            required: no
        aws_access_key:
            description: AWS access key.
            type: str
            required: no
            env:
                - name: AWS_ACCESS_KEY_ID
        aws_secret_key:
            description: AWS secret key.
            type: str
            required: no
            env:
                - name: AWS_SECRET_ACCESS_KEY
        region:
            description: AWS region to use.
            type: str
            required: no
            env:
                - name: AWS_DEFAULT_REGION
        bucket:
            description: Name of the S3 bucket
            type: str
            required: yes
            env:
                - name: ANSIBLE_INVENTORY_S3_BUCKET
        key:
            description: Key of the object contianing JSON inventory
            type: str
            required: yes
"""

import json

from ansible.errors import AnsibleError
from ansible.inventory.data import InventoryData
from ansible.plugins.inventory import BaseInventoryPlugin, Constructable

try:
    import boto3
except ImportError:
    raise AnsibleError("S3 dynamic inventory plugin requires boto3")


class InventoryModule(BaseInventoryPlugin, Constructable):
    NAME = "s3"

    def __init__(self):
        super().__init__()

    def verify_file(self, path):
        if not super().verify_file(path):
            return False
        return path.endswith(".yml") or path.endswith(".yaml")

    def parse(self, inventory: InventoryData, loader, path, cache=True):
        super().parse(inventory, loader, path, cache)

        self._read_config_data(path)
        s3_url = self.get_option("s3_url")
        access_key = self.get_option("aws_access_key")
        secret_key = self.get_option("aws_secret_key")
        region = self.get_option("region")
        bucket = self.get_option("bucket")
        key = self.get_option("key")

        s3 = boto3.resource(
            "s3",
            aws_access_key_id=access_key,
            aws_secret_access_key=secret_key,
            region_name=region,
            endpoint_url=s3_url,
        )
        obj = s3.Object(bucket, key)
        data = json.load(obj.get()["Body"])

        self._parse_inventory(data)

    def _parse_inventory(self, data):
        # Inventory data is structured like this:
        #
        # inventory = {
        #   group_name: group_data, ...
        # }
        #
        # group_data = {
        #   vars: {
        #     name: value, ...
        #   },
        #   hosts: {
        #     host_name: vars, ...
        #   },
        #   children: {
        #     group_name: group_data, ...
        #   }
        # }
        #
        for group in data:
            self._parse_group(group, data[group])

    def _parse_group(self, group, data):
        inventory = self.inventory
        group = inventory.add_group(group)

        for section in data:
            if section == "vars":
                for name, value in data[section].items():
                    inventory.set_variable(group, name, value)
                continue

            if section == "hosts":
                for host, hostvars in data[section].items():
                    host = inventory.add_host(host, group=group)
                    for name, value in hostvars.items():
                        inventory.set_variable(host, name, value)
                continue

            if section == "children":
                for subgroup in data[section]:
                    subgroup = self._parse_group(subgroup, data[section][subgroup])
                    inventory.add_child(group, subgroup)
                continue

        return group
