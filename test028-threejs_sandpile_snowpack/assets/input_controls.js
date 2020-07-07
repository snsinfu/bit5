export class InputControls {
  constructor(handler) {
    this._handler = handler;
    this._particlesInput = document.querySelector("#config-particles");
    this._sizeInput = document.querySelector("#config-size");
    this._applyButton = document.querySelector("#config-apply");
    this._runButton = document.querySelector("#run-toggle");
    this._fpsInput = document.querySelector("#fps");
    this._fpsOutput = document.querySelector("#fps-view");

    this._applyButton.addEventListener("click", _ => this._onApplyClick());
    this._runButton.addEventListener("click", _ => this._onRunClick());
    this._fpsInput.addEventListener("input", _ => this._onFPSInput());
  }

  setConfig(config) {
    let {particleCount, latticeSize, mechanicsFPS} = config;
    this._particlesInput.value = particleCount;
    this._sizeInput.value = latticeSize;
    this._fpsInput.value = mechanicsFPS;
  }

  setRunning(yes) {
    this._runButton.checked = yes;
  }

  setFPS(fps) {
    this._fpsInput.value = fps;
    this._fpsOutput.innerHTML = fps.toString();
  }

  getConfig() {
    let particleCount = parseInt(this._particlesInput.value, 10);
    let size = parseInt(this._sizeInput.value, 10);
    return {
      particleCount: particleCount,
      latticeSize: size
    };
  }

  getRunning() {
    return this._runButton.checked;
  }

  getFPS() {
    return this._fpsInput.value;
  }

  _onApplyClick() {
    let event = new CustomEvent("app:changeConfig", {detail: this.getConfig()});
    this._handler.dispatchEvent(event);
  }

  _onRunClick() {
    if (this.getRunning()) {
      this._handler.dispatchEvent(new CustomEvent("app:start"));
    } else {
      this._handler.dispatchEvent(new CustomEvent("app:pause"));
    }
  }

  _onFPSInput() {
    let fps = this.getFPS();
    this._fpsOutput.innerHTML = fps.toString();
    let event = new CustomEvent("app:changeFPS", {detail: {fps}});
    this._handler.dispatchEvent(event);
  }
}
