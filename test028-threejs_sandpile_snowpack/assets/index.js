import { SandpileSync } from "./sandpile_sync.js";
import { SandpileRugged } from "./sandpile_rugged.js";
import { GraphicalView } from "./view.js";
import { InputControls } from "./input_controls.js";


const TRANSITION_FPS = 60;
const TICK_PER_SECOND = 1000;

const DEFAULT_CONFIG = {
  particleCount: 20000,
  latticeSize: 71,
  mechanicsFPS: 12
};

var _app;


document.addEventListener("DOMContentLoaded", _ => {
  let inputs = new InputControls(window);

  window.addEventListener("app:changeConfig", event => {
    let config = event.detail;
    resetApp(config);
  });

  window.addEventListener("app:pause", _ => _app.pause());
  window.addEventListener("app:start", _ => _app.start());

  resetApp(DEFAULT_CONFIG);

  inputs.setConfig(DEFAULT_CONFIG);
  inputs.setRunning(true);
});


function resetApp(config) {
  if (_app !== undefined) {
    _app.destroy();
  }

  let viewport = document.querySelector("#viewport");
  let view = new GraphicalView(viewport, config.particleCount, config.latticeSize, config.latticeSize);
  let sandpile = new SandpileSync(config.particleCount, config.latticeSize, config.latticeSize);
  _app = new App(sandpile, view, config.mechanicsFPS);
  _app.start();
}


const STATE_PAUSED = 0;
const STATE_RUNNING = 1;
const STATE_DESTROYED = 2;


class App {
  constructor(sandpile, view, fps) {
    this._sandpile = sandpile;
    this._view = view;
    this._previousUpdateTick = performance.now();
    this._previousTransitTick = performance.now();
    this._state = STATE_PAUSED;
    this._mechanicsFPS = fps;
    this.update();
  }

  start() {
    this._state = STATE_RUNNING;
    this.animate(performance.now());
  }

  pause() {
    this._state = STATE_PAUSED;
  }

  destroy() {
    this._state = STATE_DESTROYED;
    this._view.destroy();
  }

  animate(tick) {
    if (this._state !== STATE_DESTROYED) {
      requestAnimationFrame(tick => this.animate(tick));
    }

    if ((tick - this._previousUpdateTick) * this._mechanicsFPS >= TICK_PER_SECOND) {
      if (this._state === STATE_RUNNING) {
        this.update();
      }
      this._previousUpdateTick = tick;
    }

    if ((tick - this._previousTransitTick) * TRANSITION_FPS >= TICK_PER_SECOND) {
      this._view.transit(this._sandpile.positions);
      this._previousTransitTick = tick;
    }

    this._view.render();
  }

  update() {
    let centerX = (this._sandpile.width / 2) | 0;
    let centerY = (this._sandpile.height / 2) | 0;
    this._sandpile.add(centerX, centerY);
    this._sandpile.step();
  }
};
