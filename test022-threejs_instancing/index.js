import * as THREE from "./lib/build/three.module.js";
import { TrackballControls } from "./lib/examples/jsm/controls/TrackballControls.js";


var _container;
var _width;
var _height;
var _renderer;
var _scene;
var _instances;
var _camera;
var _controls;


document.addEventListener("DOMContentLoaded", _ => {
  initRenderer();
  initScene();
  initCamera();
  animate();
});


function animate() {
  requestAnimationFrame(animate);

  updateInstances();

  _controls.update();

  render();
}


function updateInstances() {
  // Here I want to shake the box instances but _instances.setMatrixAt() does
  // not seem to update model matrix. TODO.
}


function initRenderer() {
  _container = document.querySelector("#container");
  _width = _container.clientWidth;
  _height = _container.clientHeight;

  _renderer = new THREE.WebGLRenderer({ antialias: false });
  _renderer.setPixelRatio(window.devicePixelRatio);
  _renderer.setSize(_width, _height);
  _renderer.outputEncoding = THREE.sRGBEncoding;
  _container.appendChild(_renderer.domElement);
}


function initScene() {
  _scene = new THREE.Scene();
  _scene.background = new THREE.Color(0x000000);

  let geometry = new THREE.BoxBufferGeometry(1, 1, 1);
  let material = new THREE.MeshNormalMaterial();

  let matrix = new THREE.Matrix4();
  let mesh = new THREE.InstancedMesh(geometry, material, 100000);

  for (let i = 0; i < 100000; i++) {
    randomize(matrix);
    mesh.setMatrixAt(i, matrix);
  }

  _instances = mesh;
  _scene.add(mesh);
}


function randomize(matrix) {
    let position = new THREE.Vector3();
    let rotation = new THREE.Euler();
    let quaternion = new THREE.Quaternion();
    let scale = new THREE.Vector3();

    position.x = Math.random() * 100 - 50;
    position.y = Math.random() * 100 - 50;
    position.z = Math.random() * 100 - 50;

    rotation.x = Math.random() * 2 * Math.PI;
    rotation.y = Math.random() * 2 * Math.PI;
    rotation.z = Math.random() * 2 * Math.PI;

    quaternion.setFromEuler(rotation);

    scale.x = scale.y = scale.z = Math.random();

    matrix.compose(position, quaternion, scale);
}


function initCamera() {
  _camera = new THREE.PerspectiveCamera(70, _width / _height, 1, 100);
  _camera.position.z = 30;

  _controls = new TrackballControls(_camera, _renderer.domElement);
  _controls.autoRotate = true;
}


function render() {
  _renderer.render(_scene, _camera);
}
