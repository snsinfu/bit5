// Parsing git pack file.
// https://git-scm.com/docs/pack-format

#include <stdint.h>
#include <stdio.h>



static uint32_t
decode_uint32(const uint8_t *bits)
{
    return
        ((uint32_t) bits[0] << 24) |
        ((uint32_t) bits[1] << 16) |
        ((uint32_t) bits[2] << 8) |
        (uint32_t) bits[3];
}


static void
parse_pack(const char* filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("failed to open file");
        goto clean;
    }

    // Header
    uint8_t raw_header[12];

    const uint32_t expected_magic = 0x5041434b; // "PACK"
    const uint32_t expected_version = 2;

    if (fread(raw_header, sizeof raw_header, 1, file) == 0) {
        perror("failed to read header");
        goto clean;
    }

    const uint32_t magic = decode_uint32(raw_header);
    if (magic != expected_magic) {
        printf("bad magic: %08x\n", magic);
        goto clean;
    }

    const uint32_t version = decode_uint32(raw_header + 4);
    if (version != expected_version) {
        printf("bad version: %u\n", (unsigned) version);
        goto clean;
    }

    const uint32_t n_objects = decode_uint32(raw_header + 8);
    printf("number of objects: %u\n", (unsigned) n_objects);

    // Entries
    for (uint32_t object_index = 0; object_index < n_objects; object_index++) {
        uint8_t octet;

        if (fread(&octet, 1, sizeof octet, file) == 0) {
            perror("failed to read from file");
            goto clean;
        }

        // Split upper 3 bits (type) and lower 5 bits (encoded size).
        uint8_t type = octet >> 5;
        uint8_t entry_size = octet & 0x1f;

        for (;;) {
            if (fread(&octet, 1, sizeof octet, file) == 0) {
                perror("failed to read from file");
                goto clean;
            }

            if (!(octet & 0x80)) {
                fseek(file, -1, SEEK_CUR);
                break;
            }

            entry_size <<= 7;
            entry_size |= octet & 0x7f;
        }

        const char *type_name[] = {
            [0] = "INVALID",
            [1] = "commit",
            [2] = "tree",
            [3] = "blob",
            [4] = "tag",
            [5] = "unknown (5)",
            [6] = "ofs-delta",
            [7] = "ref-delta",
        };

        printf(
            "entry %lu/%lu (%s, %llu octet):\n",
            (unsigned long) object_index + 1,
            (unsigned long) n_objects,
            type_name[type],
            (unsigned long long) entry_size
        );

        fseek(file, (fpos_t) entry_size, SEEK_CUR);
    }

clean:
    fclose(file);
}


int
main(int argc, char** argv)
{
    for (int i = 1; i < argc; i++) {
        parse_pack(argv[i]);
    }
}
