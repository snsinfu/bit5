window.addEventListener("load", () => {
  for (const card of document.querySelectorAll(".card")) {
    setup(card as HTMLElement);
  }
});

function setup(card: HTMLElement) {
  const meter = card.querySelector("meter")! as HTMLMeterElement;
  const count = card.querySelector("button.count")! as HTMLButtonElement;
  const input = card.querySelector("input[type='number']")! as HTMLInputElement;
  const steps = card.querySelectorAll("button.step");

  for (const step of steps) {
    step.addEventListener("click", () => {
      const stepSize = Number.parseInt(step.textContent!);
      input.value = (Number.parseInt(input.value) + stepSize).toString();
    });
  }

  count.addEventListener("click", () => {
    meter.value -= Number.parseInt(input.value);
    input.value = "0";
    input.max = meter.value.toString();
  });
}
