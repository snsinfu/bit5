type Clause = number[];

function main() {
  const n = 5;
  const n_vars = n * n;
  const clauses: Clause[] = [];

  for (let i = 0; i < n; i++) {
    clauses.push(...any(makeRow(i, n)));
    clauses.push(...any(makeColumn(i, n)));
  }

  for (let i = 0; i < n; i++) {
    clauses.push(...distinct(makeRow(i, n)));
    clauses.push(...distinct(makeColumn(i, n)));
  }

  //  1  2  3  4
  //  5  6  7  8
  //  9 10 11 12
  // 13 14 15 16
  //
  // ---> [ 13 ]
  //      [ 9, 14 ]
  //      [ 5, 10, 15 ]
  //      [ 1, 6, 11, 16 ]
  //      [ 2, 7, 12 ]
  //      [ 3, 8 ]
  //      [ 4 ]
  clauses.push(...distinct(makeDiagonal(0, 0, n)));
  for (let i = 1; i < n; i++) {
    clauses.push(...distinct(makeDiagonal(i, 0, n)));
    clauses.push(...distinct(makeDiagonal(0, i, n)));
  }

  //  1  2  3  4
  //  5  6  7  8
  //  9 10 11 12
  // 13 14 15 16
  //
  // ---> [ 1 ]
  //      [ 2, 5 ]
  //      [ 3, 6, 9 ]
  //      [ 4, 7, 10, 13 ]
  //      [ 8, 11, 14 ]
  //      [ 12, 15 ]
  //      [ 16 ]
  clauses.push(...distinct(makeAntiDiagonal(0, 0, n)));
  for (let i = 1; i < n; i++) {
    clauses.push(...distinct(makeAntiDiagonal(0, i, n)));
    clauses.push(...distinct(makeAntiDiagonal(i, n - 1, n)));
  }

  console.log(`c ${n}-queens`);
  console.log(`p cnf ${n_vars} ${clauses.length}`);
  for (const clause of clauses) {
    console.log(clause.join(" ") + " 0");
  }
}

function locate(i: number, j: number, n: number): number | undefined {
  if (i < 0 || i >= n || j < 0 || j >= n) {
    return undefined;
  }
  return i * n + j + 1;
}

function makeRow(i: number, n: number): number[] {
  return Array.from({ length: n }).map((_, j: number) => locate(i, j, n)!);
}

function makeColumn(j: number, n: number): number[] {
  return Array.from({ length: n }).map((_, i: number) => locate(i, j, n)!);
}

function makeDiagonal(i: number, j: number, n: number): number[] {
  const vars = Array.from({ length: n }).map(
    (_, k: number) => locate(i + k, j + k, n)
  );
  return vars.filter(v => v !== undefined) as number[];
}

function makeAntiDiagonal(i: number, j: number, n: number): number[] {
  const vars = Array.from({ length: n }).map(
    (_, k: number) => locate(i + k, j - k, n)
  );
  return vars.filter(v => v !== undefined) as number[];
}

function any(vars: number[]): Clause[] {
  return [vars];
}

function distinct(vars: number[]): Clause[] {
  const clauses: Clause[] = [];

  for (let i = 0; i < vars.length; i++) {
    for (let j = i + 1; j < vars.length; j++) {
      clauses.push([-vars[i], -vars[j]]);
    }
  }

  return clauses;
}

main();
