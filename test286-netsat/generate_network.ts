import { Random } from "./random.ts";

enum Mode {
  Up,
  Down,
}

interface Edge {
  regulator: number;
  target: number;
  mode: Mode;
}

async function main() {
  const seed = 1234321;
  const nodeCount = 32;

  const random = new Random(seed);
  const nodes = Array.from({ length: nodeCount }).map((_, i) => i + 1);
  const edges = generateEdges(nodeCount, random);

  await createCnf("network.cnf", nodes, edges);
  await createDot("network.dot", nodes, edges);
}

function generateEdges(nodeCount: number, random: Random): Array<Edge> {
  const jumpFactor = 2.5;
  const edges: Array<Edge> = [];

  for (let i = 0; i < nodeCount; i++) {
    edges.push({
      regulator: i + 1,
      target: (i + 1) % nodeCount + 1,
      mode: generateMode(random),
    });

    const used = new Set<number>();

    for (;;) {
      const step = generateStep(jumpFactor, random);
      if (step < 2) {
        break;
      }

      if (used.has(step)) {
        continue;
      }
      used.add(step);

      edges.push({
        regulator: i + 1,
        target: (i + step) % nodeCount + 1,
        mode: generateMode(random),
      });
    }
  }

  return edges;
}

function generateMode(random: Random): Mode {
  return random.uniform() > 0.5 ? Mode.Up : Mode.Down;
}

function generateStep(factor: number, random: Random): number {
  return -factor * Math.log(1 - random.uniform()) | 0;
}

async function createCnf(
  filename: string,
  nodes: Array<number>,
  edges: Array<Edge>,
) {
  const sat = await Deno.create(filename);

  const nodeCount = nodes.length;
  const clauseCount = edges.length + 1;

  // CNF header
  await writeLine(sat, `p cnf ${nodeCount} ${clauseCount}`);

  // Clause for avoiding all-false assignment
  await writeLine(sat, nodes.join(" ") + " 0");

  for (const { regulator, target, mode } of edges) {
    let clause = `-${regulator}`;

    switch (mode) {
      case Mode.Up:
        clause += ` ${target}`;
        break;

      case Mode.Down:
        clause += ` -${target}`;
        break;
    }

    await writeLine(sat, clause + " 0");
  }

  sat.close();
}

async function createDot(
  filename: string,
  nodes: Array<number>,
  edges: Array<Edge>,
) {
  const dot = await Deno.create(filename);

  await writeLine(dot, "digraph {");
  await writeLine(dot, "  graph [rankdir=LR, margin=0]");

  for (const { regulator, target, mode } of edges) {
    const attributes: Array<string> = [];

    switch (mode) {
      case Mode.Up:
        attributes.push("arrowhead=vee");
        attributes.push("color=orangered");
        break;

      case Mode.Down:
        attributes.push("arrowhead=tee");
        attributes.push("color=royalblue");
        break;
    }

    if (Math.abs(regulator - target) === 1) {
      attributes.push("constraint=false");
    }

    await writeLine(
      dot,
      `  ${regulator} -> ${target} [${attributes.join(", ")}];`,
    );
  }

  await writeLine(dot, "}");

  dot.close();
}

async function writeLine(w: Deno.Writer, s: string) {
  const utf8 = new TextEncoder();
  await w.write(utf8.encode(s + "\n"));
}

await main();
