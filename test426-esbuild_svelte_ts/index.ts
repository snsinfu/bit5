import Widget from "./Widget.svelte";

window.addEventListener("load", () => {
  const widget: Widget = new Widget({
    target: document.querySelector("main"),
    props: {
      name: "Svelte TS",
    }
  });
});
