import { IntervalSemaphore } from "./semaphore.ts";

function sleep(delay: number): Promise<void> {
  return new Promise<void>((resolve, reject) => {
    setTimeout(resolve, delay * 1000);
  });
}

const sem = new IntervalSemaphore(2);
const start = Date.now();

for (let i = 0; i < 5; i++) {
  await sem.acquire();
  console.log(Date.now() - start);
}

console.log("---")
await sleep(5);
console.log("---")

for (let i = 0; i < 5; i++) {
  await sem.acquire();
  console.log(Date.now() - start);
}
