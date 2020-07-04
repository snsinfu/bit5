import { Model } from "./model.js";
import { View } from "./view.js";
import { Controller } from "./controller.js";


const CUBE_COUNT = 100000;
const MECHANICS_DELAY = 1000 / 10;


document.addEventListener("DOMContentLoaded", _ => {
  let container = document.querySelector("#container");
  let model = new Model(CUBE_COUNT);
  let view = new View(container, CUBE_COUNT);
  let controller = new Controller(model, view, MECHANICS_DELAY);
  controller.start();
});
