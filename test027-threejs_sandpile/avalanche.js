const MAX_PILE = 16;
const PLACEHOLDER_POINT = [0, 0, 20]


export class Avalanche {
  constructor(maxParticles, width, height) {
    this._maxParticles = maxParticles;
    this._particleCount = 0;
    this._width = width;
    this._height = height;
    this._offsetX = width / 2;
    this._offsetY = height / 2;

    this._positions = new Float32Array(maxParticles * 3);
    this._positions.index = (idx) => 3 * idx;

    for (let i = 0; i < maxParticles; i++) {
      let cur = this._positions.index(i);
      this._positions[cur + 0] = PLACEHOLDER_POINT[0] - this._offsetX;
      this._positions[cur + 1] = PLACEHOLDER_POINT[1] - this._offsetY;
      this._positions[cur + 2] = PLACEHOLDER_POINT[2];
    }

    this._pile = new Int32Array(width * height * MAX_PILE);
    this._pile.index = (x, y) => (x + y * width) * MAX_PILE;
  }

  add(x, y) {
    // Allocate a new point.
    let idx = this._particleCount++;
    let posCur = this._positions.index(idx);

    // Register the new point to the cell at (x, y).
    let pileCur = this._pile.index(x, y);
    let z = ++this._pile[pileCur];
    this._pile[pileCur + z] = idx;

    this._positions[posCur + 0] = x - this._offsetX;
    this._positions[posCur + 1] = y - this._offsetY;
    this._positions[posCur + 2] = z;
  }

  step() {
    for (let x = 0; x < this._width; x++) {
      for (let y = 0; y < this._height; y++) {
        // Check the number of particles in the cell at (x, y).
        let pileCur = this._pile.index(x, y);
        let pileTop = this._pile[pileCur];

        // Topple.
        if (pileTop >= 4) {
          for (let k = 0; k < 4; k++) {
            // k    0  1  2  3
            // dx  -1 +1  0  0
            // dy   0  0 -1 +1
            let dx = (k < 2 ? -1 + 2 * k : 0);
            let dy = (k < 2 ? 0 : -5 + 2 * k);

            // Remove the top particle from the toppled cell.
            let idx = this._pile[pileCur + pileTop--];
            this._pile[pileCur]--;

            // Handle boundary. Move removed point to the placeholder position.
            if (x + dx < 0 || x + dx >= this._width ||
                y + dy < 0 || y + dy >= this._height) {
              let posCur = this._positions.index(idx);
              this._positions[posCur + 0] = PLACEHOLDER_POINT[0] - this._offsetX;
              this._positions[posCur + 1] = PLACEHOLDER_POINT[1] - this._offsetY;
              this._positions[posCur + 2] = PLACEHOLDER_POINT[2];
              continue;
            }

            // Append the removed particle to the side cell.
            let sidePileCur = this._pile.index(x + dx, y + dy);
            let sidePileTop = ++this._pile[sidePileCur];
            this._pile[sidePileCur + sidePileTop] = idx;

            // Adjust the position of the particle. The particle goes to the
            // top of the side cell.
            let posCur = this._positions.index(idx);
            this._positions[posCur + 0] = x + dx - this._offsetX;
            this._positions[posCur + 1] = y + dy - this._offsetY;
            this._positions[posCur + 2] = sidePileTop;
          }
        }
      }
    }
  }

  get positions() {
    return this._positions.slice(0, this._particleCount * 3);
  }
}
