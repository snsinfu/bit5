(function() { "use strict";
  const _rows = 29;
  const _cols = 29;
  const _palette = [
    "#440154",
    "#3a528b",
    "#20918c",
    "#5ec962",
    "#fee724"
  ];
  var _canvas;
  var _vertices;

  document.addEventListener("DOMContentLoaded", _ => {
    _canvas = document.getElementById("canvas");
    _canvas.width = 400;
    _canvas.height = 400;
    _canvas.addEventListener("click", onCanvasClick);

    _vertices = new Int8Array(_rows * _cols);
    randomize();
    window.setInterval(onTimer, 500);
  });

  function onTimer() {
    tick();
    draw();
  }

  function onCanvasClick(event) {
    let {width, height, size, xMargin, yMargin} = calculateGeometry();
    let x = Math.floor((event.clientX - xMargin) / size);
    let y = Math.floor((event.clientY - yMargin) / size);

    if (x < 0 || x >= _cols) {
      return;
    }
    if (y < 0 || y >= _rows) {
      return;
    }

    let idx = locate(x, y);
    _vertices[idx]++;

    draw();
  }

  function draw() {
    let ctx = _canvas.getContext("2d");
    let {width, height, size, xMargin, yMargin} = calculateGeometry();

    ctx.fillStyle = "black";
    ctx.fillRect(0, 0, width, height);

    ctx.lineWidth = 1;
    ctx.strokeStyle = "black";

    for (let x = 0; x < _cols; x++) {
      for (let y = 0; y < _rows; y++) {
        let idx = locate(x, y);
        let state = _vertices[idx];

        let xpos = xMargin + x * size;
        let ypos = yMargin + y * size;
        let color = _palette[Math.min(state, _palette.length - 1)];

        ctx.fillStyle = color;
        ctx.fillRect(xpos, ypos, xpos + size, ypos + size);
        ctx.strokeRect(xpos, ypos, xpos + size, ypos + size);
      }
    }
  }

  function calculateGeometry() {
    let width = _canvas.width;
    let height = _canvas.height;

    let xSpace = width / _cols;
    let ySpace = height / _rows;
    let size = Math.min(xSpace, ySpace);

    let xMargin = (width - size * _cols) / 2;
    let yMargin = (height - size * _rows) / 2;

    return {
      width: width,
      height: height,
      size: size,
      xMargin: xMargin,
      yMargin: yMargin
    };
  }

  function randomize() {
    const maxState = 4;

    for (let x = 0; x < _cols; x++) {
      for (let y = 0; y < _rows; y++) {
        let idx = locate(x, y);
        let val = Math.min(Math.floor(Math.random() * (maxState + 1)), maxState);
        _vertices[idx] = val;
      }
    }
  }

  function tick() {
    let changeset = [];

    for (let x = 0; x < _cols; x = x + 1) {
      for (let y = 0; y < _rows; y = y + 1) {
        let idx = locate(x, y);

        if (_vertices[idx] >= 4) {
          changeset.push([x, y, -4]);

          if (x > 0) {
            changeset.push([x - 1, y, 1]);
          }
          if (y > 0) {
            changeset.push([x, y - 1, 1]);
          }
          if (x < _cols - 1) {
            changeset.push([x + 1, y, 1]);
          }
          if (y < _rows - 1) {
            changeset.push([x, y + 1, 1]);
          }
        }
      }
    }

    changeset.forEach(change => {
      let [x, y, delta] = change;
      _vertices[locate(x, y)] += delta;
    });
  }

  function locate(x, y) {
    return x + _cols * y;
  }
})();
