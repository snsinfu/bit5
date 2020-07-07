export class InputControls {
  constructor(handler) {
    this._handler = handler;
    this._particlesInput = document.querySelector("#config-particles");
    this._sizeInput = document.querySelector("#config-size");
    this._fpsInput = document.querySelector("#config-fps");
    this._applyButton = document.querySelector("#config-apply");
    this._runButton = document.querySelector("#run-toggle");

    this._applyButton.addEventListener("click", _ => this._onApplyClick());
    this._runButton.addEventListener("click", _ => this._onRunClick());
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

  _onApplyClick() {
    let particleCount = parseInt(this._particlesInput.value, 10);
    let size = parseInt(this._sizeInput.value, 10);
    let fps = parseInt(this._fpsInput.value, 10);

    let event = new CustomEvent("app:changeConfig", {
      detail: {
        particleCount: particleCount,
        latticeSize: size,
        mechanicsFPS: fps
      }
    });
    this._handler.dispatchEvent(event);
  }

  _onRunClick() {
    if (this._runButton.checked) {
      this._handler.dispatchEvent(new CustomEvent("app:start"));
    } else {
      this._handler.dispatchEvent(new CustomEvent("app:pause"));
    }
  }
}
