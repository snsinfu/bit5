(function() {
  "use strict";

  window.addEventListener("load", () => {
    (new App()).start();
  });

  const modelWidth = 10;
  const modelHeight = 9;
  const canvasColor = "#f4f4f4";
  const exitColor = "#ee6666";
  const cellColor = {0: "#e0e0e0", 1: "#333333", 2: "#cccc66"};
  const cellSize = 23;
  const cellMargin = 3;
  const canvasWidth = cellSize * modelWidth + cellMargin * (modelWidth + 1);
  const canvasHeight = cellSize * modelHeight + cellMargin * (modelHeight + 1);
  const renderFPS = 60;
  const modelFPS = 120;

  class App {
    constructor() {
      this.canvas = document.getElementById("canvas");
      this.canvas.width = canvasWidth;
      this.canvas.height = canvasHeight;

      this.states = new Int8Array(modelWidth * modelHeight);
      this.states.locate = (x, y) => x + y * modelWidth;
      for (let x = 0; x < modelWidth; x++) {
        for (let y = 0; y < modelHeight; y++) {
          let initState;
          if (x < modelWidth / 2) {
            initState = Math.random() < 0.8 ? 1 : 0;
          } else {
            initState = Math.random() < 0.8 ? 2 : 0;
          }
          this.states[this.states.locate(x, y)] = initState;
        }
      }
    }

    start() {
      this.prevRenderTick = performance.now();
      this.prevModelTick = performance.now();
      this.onFrame();
    }

    update() {
      const x = Math.min(modelWidth - 1, modelWidth * Math.random()) << 0;
      const y = Math.min(modelHeight - 1, modelHeight * Math.random()) << 0;
      const location = this.states.locate(x, y);

      switch (this.states[location]) {
      case 0:
        break;

      case 1:
        this.updateStone(x, y);
        break;

      case 2:
        this.updateDroplet(x, y);
        break;
      }
    }

    updateStone(x, y) {
      const r = Math.random() * 0.4;
      const location = this.states.locate(x, y);

      if (r < 0.05 && x - 1 >= 0) {
        const adjacent = this.states.locate(x - 1, y);
        if (this.states[adjacent] === 0) {
          this.states[adjacent] = this.states[location];
          this.states[location] = 0;
        }
        return;
      }

      if (r < 0.10 && x + 1 < modelWidth) {
        const adjacent = this.states.locate(x + 1, y);
        if (this.states[adjacent] === 0) {
          this.states[adjacent] = this.states[location];
          this.states[location] = 0;
        }
        return;
      }

      if (r < 0.15 && y - 1 >= 0) {
        const adjacent = this.states.locate(x, y - 1);
        if (this.states[adjacent] === 0) {
          this.states[adjacent] = this.states[location];
          this.states[location] = 0;
        }
        return;
      }

      if (r < 0.20 && y + 1 >= 0) {
        const adjacent = this.states.locate(x, y + 1);
        if (this.states[adjacent] === 0) {
          this.states[adjacent] = this.states[location];
          this.states[location] = 0;
        }
        return;
      }
    }

    updateDroplet(x, y) {
      const r = Math.random() * 0.4;
      const midY = (modelHeight / 2) << 0;
      const location = this.states.locate(x, y);

      if (r < 0.10 && x - 1 >= 0) {
        const adjacent = this.states.locate(x - 1, y);
        if (this.states[adjacent] === 0) {
          this.states[adjacent] = this.states[location];
          this.states[location] = 0;
        }
        return;
      }

      if (r < 0.20 && x + 1 < modelWidth) {
        const adjacent = this.states.locate(x + 1, y);
        if (this.states[adjacent] === 0) {
          this.states[adjacent] = this.states[location];
          this.states[location] = 0;
        }
        return;
      }

      if (r < 0.30 && y - 1 >= 0) {
        const adjacent = this.states.locate(x, y - 1);
        if (this.states[adjacent] === 0) {
          this.states[adjacent] = this.states[location];
          this.states[location] = 0;
        }
        return;
      }

      if (r < 0.40 && y + 1 >= 0) {
        const adjacent = this.states.locate(x, y + 1);
        if (this.states[adjacent] === 0) {
          this.states[adjacent] = this.states[location];
          this.states[location] = 0;
        }
        return;
      }

      if (r < 0.50 && x === 0 && y === midY) {
        this.states[location] = 0;
        return;
      }
    }

    render() {
      const ctx = this.canvas.getContext("2d");

      ctx.fillStyle = canvasColor;
      ctx.fillRect(0, 0, canvasWidth, canvasHeight);

      for (let x = 0; x < modelWidth; x++) {
        for (let y = 0; y < modelHeight; y++) {
          const state = this.states[this.states.locate(x, y)];
          const posX = cellMargin + x * (cellSize + cellMargin);
          const posY = cellMargin + y * (cellSize + cellMargin);

          ctx.fillStyle = cellColor[state];
          ctx.fillRect(posX, posY, cellSize, cellSize);
        }
      }

      const midY = (modelHeight / 2) << 0;
      const posX = 0;
      const posY = cellMargin + midY * (cellSize + cellMargin);
      ctx.fillStyle = exitColor;
      ctx.fillRect(posX, posY, cellMargin, cellSize);
    }

    onFrame() {
      window.requestAnimationFrame(() => this.onFrame());

      const tick = performance.now();

      if ((tick - this.prevModelTick) * modelFPS >= 1000) {
        this.update();
        this.prevModelTick = tick;
      }

      if ((tick - this.prevRenderTick) * renderFPS >= 1000) {
        this.render();
        this.prevRenderTick = tick;
      }
    }
  };
})();
