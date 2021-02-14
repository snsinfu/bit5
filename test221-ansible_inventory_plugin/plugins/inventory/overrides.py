from ansible.plugins.inventory import BaseInventoryPlugin


DOCUMENTATION = """
    name: overrides
    plugin_type: inventory
    short_description: Modular inventory
    options:
        overrides:
            type: list
            default: []
"""


class InventoryModule(BaseInventoryPlugin):
    NAME = "overrides"

    def parse(self, inventory, loader, path, cache=True):
        super().parse(inventory, loader, path)

        self._read_config_data(path)

        for filename in self.get_option("overrides"):
            data = loader.load_from_file(filename)

            groups_data = data.get("groups")
            if groups_data:
                define_groups(self.inventory, groups_data)

            vars_data = data.get("vars")
            if vars_data:
                define_vars(self.inventory, vars_data)


def define_groups(inventory, groups_data):
    for group_name, group_data in groups_data.items():
        group = inventory.add_group(group_name)

        for host_name in group_data.get("hosts", []):
            inventory.add_host(host_name, group)

        for child_name in group_data.get("children", []):
            child = inventory.add_group(child_name)
            inventory.add_child(group, child)


def define_vars(inventory, vars_data):
    common_vars = vars_data.get("common", {})
    hosts_data = vars_data.get("hosts", {})
    groups_data = vars_data.get("groups", {})

    for host_name in inventory.hosts:
        for k, v in common_vars.items():
            inventory.set_variable(host_name, k, v)

    for host_name, host_vars in hosts_data.items():
        for k, v in host_vars.items():
            inventory.set_variable(host_name, k, v)

    for group_name, group_vars in groups_data.items():
        for k, v in group_vars.items():
            inventory.set_variable(group_name, k, v)
