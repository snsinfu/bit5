export interface Nop {
  kind: "nop";
}

export interface Mul {
  kind: "mul";
}

export interface Add {
  kind: "add";
}

export interface Const {
  kind: "const";
  value: number;
}

export type Op = Nop | Mul | Add | Const;


export function execute(ops: Op[], init: number[]): number[] {
  const stack = Array.from(init);

  for (const op of ops) {
    switch (op.kind) {
      case "nop":
        break;

      case "mul":
        stack.push(stack.pop()! * stack.pop()!);
        break;

      case "add":
        stack.push(stack.pop()! + stack.pop()!);
        break;

      case "const":
        stack.push(op.value);
        break;
    }
  }

  return stack;
}


export function format(ops: Op[]): string {
  const units: string[] = [];

  for (const op of ops) {
    switch (op.kind) {
      case "nop":
        units.push(".");
        break;

      case "mul":
        units.push("*");
        break;

      case "add":
        units.push("+");
        break;

      case "const":
        units.push(op.value.toString());
        break;
    }
  }

  return units.join(" ");
}
