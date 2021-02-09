import os
import yaml

from ansible.errors import AnsibleError
from ansible.inventory.group import Group
from ansible.inventory.host import Host
from ansible.plugins.vars import BaseVarsPlugin
from ansible.utils.vars import combine_vars


VARS_FILENAME = "vars.yml"
CACHE = []


def _load_vars_packs(loader, filename):
    # DataLoader does not parse multi-document yaml.
    #data = loader.load_from_file(filename)

    # Here we use a private function...
    content, _ = loader._get_file_contents(filename)
    documents = yaml.safe_load_all(content)

    packs = []
    for document in documents:
        pack = {
            key: document.get(key, {})
            for key in ["vars", "host_vars", "group_vars"]
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
            packs = CACHE = _load_vars_packs(loader, VARS_FILENAME)

        result = {}

        for pack in packs:
            for entity in entities:
                common_vars = pack["vars"]
                host_vars = pack["host_vars"]
                group_vars = pack["group_vars"]

                if isinstance(entity, Host):
                    if entity.name in host_vars:
                        result = combine_vars(result, host_vars[entity.name])

                if isinstance(entity, Group):
                    result = combine_vars(result, common_vars)
                    if entity.name in group_vars:
                        result = combine_vars(result, group_vars[entity.name])

        return result
