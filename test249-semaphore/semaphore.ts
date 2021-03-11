export class IntervalSemaphore {
  #bus: EventTarget;
  #token: number;
  #tokenMax: number;
  #delay: number;
  #timer?: number;

  constructor(tokens: number, delay: number = 1) {
    this.#bus = new EventTarget();
    this.#token = tokens;
    this.#tokenMax = tokens;
    this.#delay = delay;
  }

  async acquire() {
    this.wakeup();
    while (--this.#token < 0) {
      await listen("token", this.#bus);
    }
  }

  private wakeup() {
    if (this.#timer === undefined) {
      console.log("WAKEUP");
      this.#timer = setInterval(() => this.reload(), this.#delay * 1000);
    }
  }

  private reload() {
    if (this.#token === this.#tokenMax) {
      console.log("SLEEP");
      clearInterval(this.#timer);
      this.#timer = undefined;
    } else {
      console.log("RELOAD");
      this.#token = this.#tokenMax;
      this.#bus.dispatchEvent(new Event("token"));
    }
  }
}

function listen(type: string, bus: EventTarget = window): Promise<Event> {
  return new Promise((resolve, reject) => {
    const callback = (event: Event) => {
      bus.removeEventListener(type, callback);
      resolve(event);
    };
    bus.addEventListener(type, callback);
  });
}
