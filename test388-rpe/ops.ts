interface Mul {
  op: "mul";
}

interface Add {
  op: "add";
}

interface Constant {
  op: "constant";
  value: number;
}

type Op = Mul | Add | Constant;
