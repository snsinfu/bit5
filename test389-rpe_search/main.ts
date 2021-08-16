import {Op, execute, format} from "./ops.ts";


function main() {
  // Enumerate all 5-op binary functions.
  const args: number[] = [1, 2];
  const maxOps = 5;
  enumerateFunctions([], maxOps, args);
}


function enumerateFunctions(ops: Op[], max: number, init: number[]) {
  if (ops.length === max) {
    const result = execute(ops, init);
    if (result.length === 1 && !Number.isNaN(result[0])) {
      console.log(format(ops), "=>", result[0]);
    }
    return;
  }

  const nextOps: Op[] = [
    {kind: "nop"},
    {kind: "mul"},
    {kind: "add"},
    {kind: "const", value: 1},
  ];

  for (const op of nextOps) {
    enumerateFunctions(ops.concat([op]), max, init);
  }
}


main();
