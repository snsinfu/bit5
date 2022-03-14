import jax
import jax.numpy as jnp


def main():
    def loss(x):
        return jnp.mean(jnp.square((x - 1) * (x + 1)))

    key = jax.random.PRNGKey(0)
    init = jax.random.normal(key, shape=(10,))
    x, history = gradient_descent(loss, init, rate=1, max_iter=500, tol=1e-10)

    print(history)
    print(x)


# Note: JAX cannot JIT a function whose parameters include a function.

def gradient_descent(loss_func, init, rate, *, decay_rate=0, max_iter=100, tol=0):
    loss_grad = jax.value_and_grad(loss_func)

    def cond_func(state):
        # Note: "and" operator trips up JAX. "&" operator works.
        return (state["step"] <= max_iter) & ~(state["delta"] < tol)

    def loop_func(state):
        step = state["step"]
        x = state["x"]
        history = state["history"]

        loss, grad = loss_grad(x)
        cur_rate = rate / (1 + decay_rate * step)
        dx = cur_rate * grad
        delta = jnp.max(jnp.abs(dx))

        state = {
            "step":    step + 1,
            "delta":   delta,
            "x":       x - dx,
            "history": history.at[step].set(loss),
        }
        return state

    state = {
        "step":    0,
        "delta":   jnp.nan,
        "x":       jnp.asarray(init),
        "history": jnp.full(max_iter, jnp.nan),
    }
    state = jax.lax.while_loop(cond_func, loop_func, state)

    x = state["x"]
    history = state["history"][:state["step"]]
    return x, history


if __name__ == "__main__":
    main()
