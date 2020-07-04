import * as THREE from "./lib/build/three.module.js";


const INITIAL_RADIUS = 50;
const PHASE_RATE = 0.1;
const MOTION_STEP = 0.3;
const WOBBLE_STEP = 0.2;


export class Model {
  constructor(cubes) {
    this._time = 0;
    this._cubeCount = cubes;
    this._positions = makeRandomPositions(cubes, INITIAL_RADIUS);
    this._quaternions = makeRandomQuaternions(cubes);
  }

  tick() {
    this._time++;

    let wobbler = makeRandomQuaternion();

    for (let i = 0; i < this._cubeCount; i++) {
      let step = MOTION_STEP;
      let phase = PHASE_RATE * this._time * Math.cos(i);
      let wobble = WOBBLE_STEP * Math.cos(phase);

      this._positions[i].x += step * Math.sin(1.618 * phase);
      this._positions[i].y += step * Math.cos(0.618 * phase);
      this._positions[i].z += step * Math.sin(1.118 * phase);

      this._quaternions[i].rotateTowards(wobbler, wobble);
      this._quaternions[i].normalize();
    }
  }

  getPosition(i) {
    return this._positions[i];
  }

  getQuaternion(i) {
    return this._quaternions[i];
  }
}


function makeRandomPositions(count, radius) {
  let positions = new Array;
  for (let i = 0; i < count; i++) {
    positions.push(makeRandomPosition(radius));
  }
  return positions;
}


function makeRandomQuaternions(count) {
  let quaternions = new Array;
  for (let i = 0; i < count; i++) {
    quaternions.push(makeRandomQuaternion());
  }
  return quaternions;
}


function makeRandomPosition(radius) {
  let position = new THREE.Vector3();

  do {
    position.x = 2 * (0.5 - Math.random()) * radius;
    position.y = 2 * (0.5 - Math.random()) * radius;
    position.z = 2 * (0.5 - Math.random()) * radius;
  } while (position.length() > radius);

  return position;
}


function makeRandomQuaternion() {
  let quaternion = new THREE.Quaternion();
  let rotation = new THREE.Euler();

  rotation.x = Math.random() * 2 * Math.PI;
  rotation.y = Math.random() * 2 * Math.PI;
  rotation.z = Math.random() * 2 * Math.PI;

  quaternion.setFromEuler(rotation);

  return quaternion;
}
