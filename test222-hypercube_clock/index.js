(function() {
  "use strict";

  const TICK_PER_SECOND = 1000;

  const config = {
    hypercubeDimension: 6,
    petrieDistortion: 0.02,
    petriePermutationCode: 38,
    backgroundColor: "#333333",
    defaultVertexColor: "#777777",
    activeVertexColor: "#ff6666",
    clockFont: "24px sans-serif",
    clockColor: "#eeeeee",
    softFPS: (1 << 6) / 60.0,
    margin: 10,
    dotSize: 8,
  };

  var _clock = 0;
  var _caption;
  var _canvas;
  var _vertices;
  var _prevTick = 0;
  var _prevActive = null;

  window.addEventListener("load", () => {
    _caption = document.getElementById("caption");
    _canvas = document.getElementById("canvas");

    _vertices = normalizeCoordinates(
      makeHypercubeVertices(config.hypercubeDimension, config.petrieDistortion),
      config.margin,
      config.margin,
      _canvas.width - config.margin,
      _canvas.height - config.margin
    );

    requestAnimationFrame(animate);
  });

  function animate(tick) {
    requestAnimationFrame(animate);

    if ((tick - _prevTick) * config.softFPS >= TICK_PER_SECOND) {
      render();
      _prevTick = tick;
    }
  }

  function render() {
    const ctx = _canvas.getContext("2d");

    ctx.fillStyle = config.backgroundColor;
    ctx.fillRect(0, 0, _canvas.width, _canvas.height);

    const grayClock = (_clock ^ (_clock >> 1)) ^ config.petriePermutationCode;
    const vertexCount = _vertices.length;

    for (let i = 0; i < vertexCount; i++) {
      const [x, y] = _vertices[i];
      const isActive = (i === grayClock);

      if (isActive) {
        ctx.fillStyle = config.activeVertexColor;
      } else {
        ctx.fillStyle = config.defaultVertexColor;
      }

      ctx.fillRect(
        x - config.dotSize / 2,
        y - config.dotSize / 2,
        config.dotSize,
        config.dotSize
      );

      if (isActive && _prevActive !== null) {
        const [px, py] = _vertices[_prevActive];
        ctx.strokeStyle = config.activeVertexColor;
        ctx.beginPath();
        ctx.moveTo(x, y);
        ctx.lineTo(px, py);
        ctx.stroke();
      }

      if (isActive) {
        _prevActive = i;
      }
    }

    ctx.textBaseline = "top";
    ctx.fillStyle = config.clockColor;
    ctx.font = config.clockFont;
    ctx.fillText(`${_clock}`, 0, 0);

    if (++_clock === vertexCount) {
      _clock = 0;
    }
  }

  function normalizeCoordinates(points, loX, loY, upX, upY) {
    const minX = points.reduce((min, pt) => Math.min(min, pt[0]), Infinity);
    const minY = points.reduce((min, pt) => Math.min(min, pt[1]), Infinity);
    const maxX = points.reduce((max, pt) => Math.max(max, pt[0]), -Infinity);
    const maxY = points.reduce((max, pt) => Math.max(max, pt[1]), -Infinity);

    return points.map(pt => {
      let [x, y] = pt;
      x = loX + (upX - loX) * (x - minX) / (maxX - minX);
      y = loY + (upY - loY) * (y - minY) / (maxY - minY);
      return [x, y];
    });
  }

  function makeHypercubeVertices(dimension, distortion) {
    const vertexCount = 1 << dimension;
    const vertices = [];
    const basis = makeHypercubeBasis(dimension, distortion);

    for (let i = 0; i < vertexCount; i++) {
      let x = 0;
      let y = 0;

      for (let k = 0; k < dimension; k++) {
        if (i & (1 << k)) {
          const [dx, dy] = basis[k];
          x += dx;
          y += dy;
        }
      }

      vertices.push([x, y]);
    }

    return vertices;
  }

  function makeHypercubeBasis(dimension, distortion) {
    const basis = [];

    for (let i = 0; i < dimension; i++) {
      const angle = (Math.PI / dimension + distortion) * i;
      const x = Math.cos(angle);
      const y = Math.sin(angle);
      basis.push([x, y]);
    }

    return basis;
  }
})();
