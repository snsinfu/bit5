# 4k aliasing experiment

The program reads values from a float array, skip some interval and write new
values to the other place in the same array. I saw ~20% performance drops when
the read-write skip interval is a multiple of 1024 floats (i.e., 4KiB).

| skip | throughput (call/s) |
|------|---------------------|
| 1008 | 113.9               |
| 1024 |  95.2               |
| 1040 | 114.7               |
| 4096 |  95.4               |
| 4112 | 113.7               |

### References

- https://richardstartin.github.io/posts/4k-aliasing
