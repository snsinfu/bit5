const CANVAS_SELECTOR = "#canvas";
const FRAME_DELAY = 100;
const WIDTH = 800;
const HEIGHT = 600;
const COLUMNS = 40;


window.addEventListener("DOMContentLoaded", _ => {
  let canvas = document.querySelector(CANVAS_SELECTOR);
  let view = new View(canvas);
  let domain = new Domain(view);
  domain.start();
});


class Domain {
  constructor(view) {
    this._view = view;
  }

  start() {
    this.animate(performance.now());
  }

  animate(currentTick) {
    requestAnimationFrame(t => this.animate(t));

    if (this._previousTick === undefined ||
        currentTick - this._previousTick >= FRAME_DELAY) {
      this._view.setPhase(currentTick / FRAME_DELAY);
      this._view.render();
      this._previousTick = currentTick;
    }
  }
}


class View {
  constructor(canvas) {
    canvas.width = WIDTH;
    canvas.height = HEIGHT;

    this._canvas = canvas;
    this._phase = 0;
  }

  setPhase(phase) {
    this._phase = phase;
  }

  render() {
    let ctx = this._canvas.getContext("2d");
    let width = this._canvas.width;
    let height = this._canvas.height;

    ctx.fillStyle = "black";
    ctx.fillRect(0, 0, width, height);

    let boxSize = width / COLUMNS;
    let rows = Math.trunc(height / boxSize);
    let count = COLUMNS * rows;

    for (let i = 0; i < count; i++) {
      let x = boxSize * (i % COLUMNS);
      let y = boxSize * Math.trunc(i / COLUMNS);

      let [r, g, b] = synthesizeRGB(i, this._phase);

      ctx.fillStyle = `rgb(${r},${g},${b})`;
      ctx.fillRect(x + 1, y + 1, boxSize - 2, boxSize - 2);
    }
  }
}


function synthesizeRGB(i, phase) {
  let r = 0.75 + 0.25 * Math.sin(1.618 * i + phase + 0.618);
  let g = 0.75 + 0.25 * Math.sin(0.618 * i * phase + 1.118);
  let b = 0.75 + 0.25 * Math.sin(1.118 * i / phase + 1.618);

  r = Math.trunc(r * 255.99);
  g = Math.trunc(g * 255.99);
  b = Math.trunc(b * 255.99);

  return [r, g, b];
}
