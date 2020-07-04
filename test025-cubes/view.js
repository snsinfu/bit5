import * as THREE from "./lib/build/three.module.js";
import { TrackballControls } from "./lib/examples/jsm/controls/TrackballControls.js";


export class View {
  constructor(container, cubes) {
    this._container = container;

    this.initRenderer();
    this.initScene(cubes);
    this.initCamera();
  }

  initRenderer() {
    let width = this._container.clientWidth;
    let height = this._container.clientHeight;

    let renderer = new THREE.WebGLRenderer({ antialias: false });
    renderer.setPixelRatio(window.devicePixelRatio);
    renderer.setSize(width, height);
    renderer.outputEncoding = THREE.sRGBEncoding;

    this._container.appendChild(renderer.domElement);
    this._renderer = renderer;
  }

  initScene(cubes) {
    this._scene = new THREE.Scene();
    this._scene.background = new THREE.Color(0x000000);
    this._cubes = makeCubes(cubes);
    this._scene.add(this._cubes);
  }

  initCamera() {
    let size = new THREE.Vector2();
    this._renderer.getSize(size);
    let aspect = size.x / size.y;

    this._camera = new THREE.PerspectiveCamera(100, aspect, 1, 1000);
    this._camera.position.z = 100;

    this._controls = new TrackballControls(this._camera, this._renderer.domElement);
  }

  updateCubes(matrix) {
    for (let i = 0; i < this._cubes.count; i++) {
      this._cubes.setMatrixAt(i, matrix(i));
    }
    this._cubes.instanceMatrix.needsUpdate = true;
  }

  render() {
    this._controls.update();
    this._renderer.render(this._scene, this._camera);
  }
}


function makeCubes(count) {
  let geometry = new THREE.BoxBufferGeometry(1, 1, 1);
  let material = new THREE.MeshNormalMaterial();
  return new THREE.InstancedMesh(geometry, material, count);
}
