import * as THREE from "./lib/build/three.module.js";


export class Controller {
  constructor(model, view, delay) {
    this._model = model;
    this._view = view;
    this._delay = delay;
  }

  start() {
    this.animate(performance.now());
  }

  animate(tick) {
    requestAnimationFrame(t => this.animate(t));

    let shouldComputeMechanics = (
      this._previousTick === undefined || tick - this._previousTick >= this._delay
    );
    if (shouldComputeMechanics) {
      this._model.tick();

      this._view.updateCubes(i => {
        let matrix = new THREE.Matrix4();
        let position = this._model.getPosition(i);
        let quaternion = this._model.getQuaternion(i);
        let scale = new THREE.Vector3(1, 1, 1);
        matrix.compose(position, quaternion, scale);
        return matrix;
      });

      this._previousTick = tick;
    }

    this._view.render();
  }
}
