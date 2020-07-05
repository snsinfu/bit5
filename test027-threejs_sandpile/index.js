import { Avalanche } from "./avalanche.js";
import { GraphicalView } from "./view.js";


const PARTICLE_COUNT = 10000;
const MECHANICS_FPS = 12;
const TRANSITION_FPS = 60;
const TICK_PER_SECOND = 1000;
const GRID_WIDTH = 71;
const GRID_HEIGHT = 71


document.addEventListener("DOMContentLoaded", _ => {
  let viewport = document.querySelector("#viewport");
  let view = new GraphicalView(viewport, PARTICLE_COUNT, GRID_WIDTH, GRID_HEIGHT);
  let avalanche = new Avalanche(PARTICLE_COUNT, GRID_WIDTH, GRID_HEIGHT);
  let app = new App(avalanche, view);
  app.start();
});


class App {
  constructor(avalanche, view) {
    this._avalanche = avalanche;
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
      this._view.transit(this._avalanche.positions);
      this._previousTransitTick = tick;
    }
    this._view.render();
  }

  update() {
    let centerX = (GRID_WIDTH / 2) | 0;
    let centerY = (GRID_HEIGHT / 2) | 0;
    this._avalanche.add(centerX, centerY);
    this._avalanche.step();
  }
};
