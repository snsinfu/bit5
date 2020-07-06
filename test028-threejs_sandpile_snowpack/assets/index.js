import { SandpileSync } from "./sandpile_sync.js";
import { SandpileRugged } from "./sandpile_rugged.js";
import { GraphicalView } from "./view.js";


const PARTICLE_COUNT = 10000;
const MECHANICS_FPS = 12;
const TRANSITION_FPS = 60;
const TICK_PER_SECOND = 1000;
const LATTICE_WIDTH = 71;
const LATTICE_HEIGHT = 71


document.addEventListener("DOMContentLoaded", _ => {
  let viewport = document.querySelector("#viewport");
  let view = new GraphicalView(viewport, PARTICLE_COUNT, LATTICE_WIDTH, LATTICE_HEIGHT);
  let sandpile = new SandpileSync(PARTICLE_COUNT, LATTICE_WIDTH, LATTICE_HEIGHT);
  let app = new App(sandpile, view);
  app.start();
});


class App {
  constructor(sandpile, view) {
    this._sandpile = sandpile;
    this._view = view;
    this._previousUpdateTick = performance.now();
    this._previousTransitTick = performance.now();
    this.update();
  }

  start() {
    this.animate(performance.now());
  }

  animate(tick) {
    requestAnimationFrame(tick => this.animate(tick));

    if ((tick - this._previousUpdateTick) * MECHANICS_FPS >= TICK_PER_SECOND) {
      this.update();
      this._previousUpdateTick = tick;
    }

    if ((tick - this._previousTransitTick) * TRANSITION_FPS >= TICK_PER_SECOND) {
      this._view.transit(this._sandpile.positions);
      this._previousTransitTick = tick;
    }

    this._view.render();
  }

  update() {
    let centerX = (LATTICE_WIDTH / 2) | 0;
    let centerY = (LATTICE_HEIGHT / 2) | 0;
    this._sandpile.add(centerX, centerY);
    this._sandpile.step();
  }
};
