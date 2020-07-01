import functools
import os

from ansible.plugins.vars import BaseVarsPlugin
from ansible.utils.vars import merge_hash


DOCUMENTATION = """
vars: project_vars
short_description: Merge variables from a file or files in a directory
options:
  path:
    type: str
    ini:
      - key: project_vars
        section: vars
    env:
      - name: ANSIBLE_PROJECT_VARS
"""

DEFAULT_PATH = "vars"


@functools.lru_cache(maxsize=32)
def find_vars_files(loader, basedir, path):
    return loader.find_vars_files(basedir, path)


class VarsModule(BaseVarsPlugin):
    def get_vars(self, loader, path, entities):
        super().get_vars(loader, path, entities)

        # Locate files relative to the current working directory, not the
        # playbook directory. This is consistent with other configurations
        # like ANSIBLE_ROLES_PATH.
        basedir = os.getcwd()

        # TODO: Change to get_option once Ansible 2.10 is widely available.
        vars_path = os.getenv("ANSIBLE_PROJECT_VARS") or DEFAULT_PATH
        vars_files = find_vars_files(loader, basedir, vars_path)

        merged_vars = {}
        for file_path in sorted(vars_files):
            loaded_vars = loader.load_from_file(file_path)
            merged_vars = merge_hash(merged_vars, loaded_vars)

        return merged_vars
