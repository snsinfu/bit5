import os

from ansible.errors import AnsibleError
from ansible.inventory.group import Group
from ansible.inventory.host import Host
from ansible.plugins.vars import BaseVarsPlugin
from ansible.utils.vars import combine_vars

try:
    import boto3
except:
    raise AnsibleError("s3vars plugin requires boto3")


CACHE = None


def _fetch_vars_packs(loader):
    config = os.getenv("ANSIBLE_S3VARS_CONFIG")
    bucket = os.getenv("ANSIBLE_S3VARS_BUCKET")
    region = os.getenv("ANSIBLE_S3VARS_REGION")

    config_data = loader.load_from_file(config)

    access_key = config_data.get("aws_access_key")
    secret_key = config_data.get("aws_secret_key")
    region = region or config_data.get("region")
    bucket = bucket or config_data["bucket"]
    objects = config_data["objects"]

    s3 = boto3.resource(
        "s3",
        aws_access_key_id=access_key,
        aws_secret_access_key=secret_key,
        region_name=region,
    )

    packs = []

    for object_spec in objects:
        object_key = object_spec["key"]
        is_optional = object_spec.get("optional", False)

        try:
            obj = s3.Object(bucket, object_key)
            object_body = obj.get()["Body"]
        except s3.meta.client.exceptions.ClientError:
            # FIXME. We should not ignore non-404 errors. I thought catching
            # NoSuchKey would do, but it did not work. For some reason, S3
            # returns 403 (AccessDenied) error on fetching non-existing key.
            if is_optional:
                continue
            raise

        filename = "<s3>" + object_key
        data = loader.load(object_body, file_name=filename)

        pack = {
            "vars": data.get("vars", {}),
            "host_vars": data.get("host_vars", {}),
            "group_vars": data.get("group_vars", {}),
        }
        packs.append(pack)

    return packs


class VarsModule(BaseVarsPlugin):
    def get_vars(self, loader, path, entities, cache=True):
        super().get_vars(loader, path, entities)

        global CACHE

        if cache and CACHE:
            packs = CACHE
        else:
            packs = CACHE = _fetch_vars_packs(loader)

        result = {}

        for entity in entities:
            for pack in packs:
                common_vars = pack["vars"]
                host_vars = pack["host_vars"]
                group_vars = pack["group_vars"]

                # Our expected precedence is host > group > common vars. To
                # achieve this, we need to merge common vars *only* when group
                # vars are requested. This is because Ansible first requests
                # group vars and then host vars, overwriting the former. So,
                # if we merge common vars when host vars are requested, group
                # vars would be overwritten by the common vars, ending up the
                # wrong precedence: host > common > group.
                #
                # Hence, common vars are merged only when entity is a Group.

                if isinstance(entity, Host):
                    if entity.name in host_vars:
                        result = combine_vars(result, host_vars[entity.name])

                if isinstance(entity, Group):
                    result = combine_vars(result, common_vars)
                    if entity.name in group_vars:
                        result = combine_vars(result, group_vars[entity.name])

        return result
