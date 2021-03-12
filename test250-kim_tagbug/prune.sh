#!/bin/sh -eu

kim image ls |
awk '
$1 ~ /^localhost\// {
    name = $1
    tag = $2
    id = $3

    if (tag == "latest") {
        # Do not set 0 when the tag is not latest because image ID may repeat.
        latest_images[id] = 1
    }
    image_ids[id] = 1
    image_name[id] = name ":" tag
}
END {
    for (id in image_ids) {
        if (!latest_images[id]) {
            print id, image_name[id]
        }
    }
}
' |
while read id name; do
    echo "Removing ${id} (${name})"
    kim image rm "${id}" || true
done
