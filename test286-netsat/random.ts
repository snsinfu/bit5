export class Random {
  private a: number;
  private b: number;
  private c: number;
  private counter: number;

  constructor(seed: number) {
    this.a = 0;
    this.b = (seed & 0xFFFFFFFF);
    this.c = seed >>> 32;
    this.counter = 1;

    for (let i = 0; i < 12; i++) {
      this.generate();
    }
  }

  generate(): number {
    function rot(x: number, n: number): number {
      return (x << n) | (x >>> (32 - n));
    }

    let { a, b, c, counter } = this;

    const output = (a + b + counter) | 0;
    a = b ^ (b >>> 9);
    b = (c + (c << 3)) | 0;
    c = (output + rot(c, 21)) | 0;
    counter = (counter + 1) | 0;

    this.a = a;
    this.b = b;
    this.c = c;
    this.counter = counter;

    return output;
  }

  uniform(): number {
    return this.generate() / 0x100000000 + 0.5;
  }
}
