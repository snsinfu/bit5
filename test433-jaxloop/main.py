import jax
import jax.numpy as jnp
import numpy as np


def main():
    def loss(x):
        return jnp.mean(((x - 1) * (x + 1)) ** 2)

    init = jnp.array(np.random.normal(0, 1, size=10))
    x, history = gradient_descent(loss, init, 0.01, decay_rate=0.1)
    print(x)


# @jax.jit
def gradient_descent(loss_func, init, rate, *, decay_rate=0, max_iter=100):
    loss_grad = jax.value_and_grad(loss_func)

    def cond_func(state):
        return state["step"] <= max_iter

    def loop_func(state):
        step = state["step"]
        x = state["x"]
        history = state["history"]

        loss, grad = loss_grad(x)
        cur_rate = rate / (1 + decay_rate * step)
        dx = cur_rate * grad

        state = {
            "step":    step + 1,
            "x":       x + dx,
            "history": history.at[step].set(loss),
        }
        return state

    state = {
        "step":    0,
        "x":       jnp.asarray(init),
        "history": jnp.full(max_iter, jnp.nan),
    }
    state = jax.lax.while_loop(cond_func, loop_func, state)

    return state["x"], state["history"]


if __name__ == "__main__":
    main()
main()
