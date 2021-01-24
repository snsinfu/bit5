import os

from ansible.errors import AnsibleError
from ansible.inventory.group import Group
from ansible.inventory.host import Host
from ansible.plugins.vars import BaseVarsPlugin
from ansible.utils.vars import combine_vars


VARS_FILENAME = "vars.yml"
CACHE = None


def _load_vars_pack(loader, filename):
    data = loader.load_from_file(filename)
    return {key: data.get(key, {}) for key in ["vars", "host_vars", "group_vars"]}


class VarsModule(BaseVarsPlugin):
    def get_vars(self, loader, path, entities, cache=True):
        super().get_vars(loader, path, entities)

        global CACHE

        if cache and CACHE:
            pack = CACHE
        else:
            pack = CACHE = _load_vars_pack(loader, VARS_FILENAME)

        result = {}

        for entity in entities:
            common_vars = pack["vars"]
            host_vars = pack["host_vars"]
            group_vars = pack["group_vars"]

            if isinstance(entity, Host):
                if entity.name in host_vars:
                    result = combine_vars(result, host_vars[entity.name])

            if isinstance(entity, Group):
                # Expected precedence: common < group < host vars. To this
                # end, we need to merge common vars *only* when group vars
                # are requested. This is because Ansible first requests group
                # vars and then host vars, overwriting the former.
                result = combine_vars(result, common_vars)

                if entity.name in group_vars:
                    result = combine_vars(result, group_vars[entity.name])

        return result
