#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


static char const memory_name[] = "/2a76e741-9e8c-4bda-9758-7c2064f97a7c";
static void *const memory_addr = (void *) 0xa0000000;
static size_t const memory_size = 16uL * 1024uL * 1024uL;


int main()
{
    int rc = 1;
    int shm = -1;

    shm = shm_open(memory_name, O_CREAT | O_RDWR, 0600);
    if (shm < 0) {
        perror("shm_open");
        goto clean;
    }

    if (ftruncate(shm, memory_size) == -1) {
        perror("ftruncate");
        goto clean;
    }

    void *const memory = mmap(
        memory_addr, memory_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0
    );
    if (memory == NULL) {
        perror("mmap");
        goto clean;
    }

    if (close(shm) == -1) {
        perror("close");
        goto clean;
    }
    shm = -1;

    printf("Address = %p\n", memory);

    uint32_t *dwords = memory;
    printf("[0] %08" PRIu32 "\n", dwords[0]);
    printf("[1] %08" PRIu32 "\n", dwords[1]);
    printf("[2] %08" PRIu32 "\n", dwords[2]);

    // Mark success.
    rc = 0;

clean:
    close(shm);
    shm_unlink(memory_name);

    return rc;
}
