import * as ops from "./ops.ts";


const ops: ops.Op[] = [
  {kind: "mul"},
  {kind: "constant", value: 3},
  {kind: "add"},
  {kind: "constant", value: 4},
  {kind: "mul"},
];

const stack: number[] = [
  1, 2,
];

for (const op of ops) {
  switch (op.kind) {
    case "mul":
      stack.push(stack.pop()! * stack.pop()!);
      break;

    case "add":
      stack.push(stack.pop()! + stack.pop()!);
      break;

    case "constant":
      stack.push(op.value);
      break;
  }

  console.log(stack);
}
