Ansible custom vars plugin and templating.

- Templating works in vars plugin. Looks like, it is the caller that renders
  templates, not the `loader`. Because templating still works if variables are
  loaded with `json.load`.
- Inventory vars may refer to variables introduced in custom vars plugin.
