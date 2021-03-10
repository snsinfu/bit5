function listen(target: EventTarget, type: string): Promise<Event> {
  return new Promise<Event>((resolve, reject) => {
    const callback = (event: Event) => {
      target.removeEventListener(type, callback);
      resolve(event);
    };
    target.addEventListener(type, callback);
  });
}

class ResourceToken {
  private _tokens: number;
  private _event: EventTarget;
  private _timer: number;

  constructor(tokens: number, delay: number = 1) {
    this._tokens = tokens;
    this._event = new EventTarget();

    const tokenEvent = new CustomEvent("token");
    const reloadTokens = () => {
      this._tokens = tokens;
      this._event.dispatchEvent(tokenEvent);
    };
    this._timer = setInterval(reloadTokens, delay * 1000);
  }

  close() {
    clearInterval(this._timer);
  }

  async acquire() {
    while (--this._tokens < 0) {
      await listen(this._event, "token");
    }
  }
}

// Rate-limit acquire() to 2 calls per second.
const dbToken = new ResourceToken(2);

await dbToken.acquire();
console.log(Date.now());

await dbToken.acquire();
console.log(Date.now());

await dbToken.acquire();
console.log(Date.now());

await dbToken.acquire();
console.log(Date.now());

// FIXME: close() is needed. Otherwise deno main loop does not stop because of
// the interval timer. Can I get rid of this?
dbToken.close();
