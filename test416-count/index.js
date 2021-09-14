"use strict";
window.addEventListener("load", () => {
    for (const card of document.querySelectorAll(".card")) {
        setup(card);
    }
});
function setup(card) {
    const meter = card.querySelector("meter");
    const count = card.querySelector("button.count");
    const input = card.querySelector("input[type='number']");
    const steps = card.querySelectorAll("button.step");
    for (const step of steps) {
        step.addEventListener("click", () => {
            const stepSize = Number.parseInt(step.textContent);
            input.value = (Number.parseInt(input.value) + stepSize).toString();
        });
    }
    count.addEventListener("click", () => {
        meter.value -= Number.parseInt(input.value);
        input.value = "0";
        input.max = meter.value.toString();
    });
}
