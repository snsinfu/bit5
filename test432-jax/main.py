import jax
import jax.numpy as jnp


def main():
    # Loss function can be a method. And it's jitted!
    loss = Loss(dr=0.01, ndiv=500, scale=0.5)

    x, history = gradient_descent(
        loss.evaluate_loss,
        loss.default_guess,
        rate=10,
        max_iter=10000,
        decay_rate=1e-4,
    )
    ur, vr, uk, vk = x

    print("Loss (init):", history[0])
    print("Loss (term):", history[-1])
    print("ur:", ur)


class Loss:
    """
    Some complex loss function. There are four vector variables ur, vr, uk, vk
    where uk and vk are the Fourier (or cosine) transformations of ur and vk,
    respectively. We call the former variables k-space variables and the latter
    r-space variables. Loss function is defined in each space.
    """

    def __init__(self, dr, ndiv, scale):
        self._default_guess = jnp.zeros((4, ndiv))
        self._r_kernel = jnp.expm1(-(jnp.arange(ndiv) + 0.5) * dr / scale)
        self._fourier = make_fourier_kernel(dr, ndiv)

    @property
    def default_guess(self):
        return self._default_guess

    def evaluate_loss(self, x):
        ur, vr, uk, vk = x
        lossify = lambda e: jnp.mean(jnp.square(e))
        loss = lossify(self._rspace_error(ur, vr))
        loss += lossify(self._kspace_error(uk, vk))
        loss += lossify(self._fourier_error(ur, uk))
        loss += lossify(self._fourier_error(vr, vk))
        return loss

    def _rspace_error(self, ur, vr):
        # r-space loss.
        return vr - (ur - vr + 1) * self._r_kernel

    def _kspace_error(self, uk, vk):
        # k-space loss.
        return uk - (vk + uk * vk)

    def _fourier_error(self, fr, fk):
        # Loss for maintaining Fourier transformation relations.
        return fk - self._fourier @ fr


def make_fourier_kernel(dr, ndiv):
    dk = 2 * jnp.pi / (dr * ndiv)
    r = (jnp.arange(ndiv) + 0.5) * dr
    k = (jnp.arange(ndiv) + 0.5) * dk
    fourier = 2 * jnp.cos(jnp.outer(k, r)) * dr
    return fourier


def gradient_descent(loss_func, x, *, rate, max_iter, decay_rate=0, tol=0):
    """
    Generic gradient descent.
    """
    evaluate_loss = jax.jit(jax.value_and_grad(loss_func))
    history = []

    for step in range(max_iter):
        loss, grad = evaluate_loss(x)
        delta = grad * (rate / (1 + decay_rate * step))
        if tol and jnp.max(jnp.abs(delta)) < tol:
            break
        x -= delta
        history.append(loss)

    loss, _ = evaluate_loss(x)
    history.append(loss)

    return x, history


if __name__ == "__main__":
    main()
