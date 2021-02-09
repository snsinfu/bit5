DOCUMENTATION = '''
    vars: testing
    short_description: Test plugin
    options:
      index:
        ini:
          - key: index
            section: vars_testing
        env:
          - name: ANSIBLE_VARS_INDEX
'''

from ansible.plugins.vars import BaseVarsPlugin


class VarsModule(BaseVarsPlugin):
    def get_vars(self, loader, path, entities, cache=True):
        super().get_vars(loader, path, entities)

        # get_option is available since Ansible 2.10.
        index = self.get_option("index")

        return {"vars_testing_index": index}
