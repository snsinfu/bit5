// Parse zip file and list files.
//
// https://users.cs.jmu.edu/buchhofp/forensics/formats/pkzip-printable.html
// Note: Everything is little endian in a zip file.

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


uint32_t magic_32(uint8_t x1, uint8_t x2, uint8_t x3, uint8_t x4)
{
    // Little endian
    return (uint32_t) x1 |
        ((uint32_t) x2 << 8) |
        ((uint32_t) x3 << 16) |
        ((uint32_t) x4 << 24);
}

int main()
{
    FILE *file = fopen("_man.zip", "rb");

    enum { buffer_size = 1024 };
    enum { chunk_size = 256 };
    uint8_t buffer[buffer_size];

    // Find the end-of-central-directory record.
    const uint32_t eocd_signature = magic_32('P', 'K', 0x05, 0x06);

    struct __attribute__((packed)) eocd_record
    {
        uint32_t signature;
        uint16_t disk_number;
        uint16_t cd_start;
        uint16_t disk_entries;
        uint16_t total_entries;
        uint32_t cd_size;
        uint32_t cd_offset;
        uint16_t comment_length;
        char     data[];
    };
    struct eocd_record eocd_record;

    fseek(file, -(off_t) chunk_size, SEEK_END);
    fread(buffer, 1, chunk_size, file);

    for (size_t i = 0; i < chunk_size - 4; i++) {
        if (*(uint32_t *) (buffer + i) == eocd_signature) {
            eocd_record = *(struct eocd_record *) (buffer + i);
        }
    }

    // Now let us jump to the central directory and read file headers.
    fseek(file, (off_t) eocd_record.cd_offset, SEEK_SET);

    const uint32_t file_header_signature = magic_32('P', 'K', 0x01, 0x02);

    struct __attribute__((packed)) file_header
    {
        uint32_t signature;
        uint16_t version;
        uint16_t version_needed;
        uint16_t flags;
        uint16_t compression;
        uint16_t modification_time;
        uint16_t modification_date;
        uint32_t crc32;
        uint32_t compressed_size;
        uint32_t uncompressed_size;
        uint16_t filename_length;
        uint16_t extra_length;
        uint16_t comment_length;
        uint16_t disk_number;
        uint16_t internal_attributes;
        uint32_t external_attributes;
        uint32_t local_header_offset;
        char     data[];
    };
    struct file_header header;

    off_t cd_size = (off_t) eocd_record.cd_size;

    for (uint32_t entry = 0; entry < eocd_record.total_entries; entry++) {
        if (cd_size <= 0) {
            puts("broken central directory?");
            return 1;
        }

        fread(&header, sizeof header, 1, file);

        if (header.signature != file_header_signature) {
            puts("not a file header");
            return 1;
        }

        cd_size -= sizeof header;
        cd_size -= header.filename_length;
        cd_size -= header.extra_length;
        cd_size -= header.comment_length;

        char filename[256];
        char extra[1024];
        char comment[1024];

        fread(filename, 1, header.filename_length, file);
        fread(extra, 1, header.extra_length, file);
        fread(comment, 1, header.comment_length, file);

        printf(
            "%u\t%.*s\n",
            header.uncompressed_size,
            (int) header.filename_length,
            filename
        );
    }

    if (cd_size != 0) {
        puts("there seems to be excess entries...");
        return 1;
    }

    return 0;
}
