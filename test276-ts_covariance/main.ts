type State = Record<string, any>;

class Context<S extends State = State> {
  public state: S;

  constructor(state: S) {
    this.state = state;
  }
}

// Note that view() uses Context type with "base" generic state.
function view(context: Context) {
}

interface ControllerState extends State {
  url: string;
}

const context = new Context<ControllerState>({ url: "" });

// Context<ControllerState> is different type than Context<State>, but is
// convertible to the latter because ControllerState is convertible to State.
view(context);
