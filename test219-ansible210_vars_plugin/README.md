# Ansible vars plugin: Custom options

From Ansible 2.10 `BaseVarsPlugin` *is-a* `AnsiblePlugin`, thus supporting the
unified plugin configuration through ansible.cfg and environment variables.

- https://github.com/ansible/ansible/pull/61078
- [BaseVarsPlugin @ 2.9](https://github.com/ansible/ansible/blob/stable-2.9/lib/ansible/plugins/vars/__init__.py)
- [BaseVarsPlugin @ 2.10](https://github.com/ansible/ansible/blob/stable-2.10/lib/ansible/plugins/vars/__init__.py)

Declare plugin options in `DOCUMENTATION` as follows:

```yaml
vars: testing
short_description: Test plugin
options:
  index:
    ini:
      - key: index
        section: vars_testing
    env:
      - name: ANSIBLE_VARS_INDEX
```

This enables configuration of the vars plugin in ansible.cfg:

```toml
[vars_testing]
index = path/to/index
```

Get options by calling `self.get_option()` in a plugin implementation:

```python
def get_vars(self, loader, path, entities, cache=True):
    super().get_vars(loader, path, entities)

    # This obtains the `index` plugin option configured in ansible.cfg
    # or environment variable.
    index = self.get_option("index")

    ...
```
