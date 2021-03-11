# Async semaphore for rate-limiting code execution

```console
$ deno run main.ts
WAKEUP
1
1
RELOAD
1012
1012
RELOAD
2006
---
RELOAD
SLEEP
---
WAKEUP
7011
7011
RELOAD
8014
8014
RELOAD
9014
RELOAD
SLEEP
```
