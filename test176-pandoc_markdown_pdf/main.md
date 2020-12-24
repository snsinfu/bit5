# Diffusion coefficient and velocity autocorrelation function

## Mean-squared displacement

Let ${ x(t) }$ be the stochastic trajectory of a particle. We assume that the
trajectory is probabilistically equivalent under time shift. Then, we define
the mean-squared displacement (MSD), denoted ${ \overline{\Delta x^2}(\tau) }$,
of the particle with lag time ${ \tau }$:
$$
    \overline{\Delta x^2}(\tau)
    =
    \left\langle
        x(\tau) - x(0)
    \right\rangle .
$$
The average is taken with respect to possible instances of the stochastic
trajectories. MSD represents how "fast" the particle can diffuse, but it
encodes far deeper information as we see below.


## Diffusion coefficient

Consider the scaled slope of MSD:
$$
    D(\tau)
    =
    \frac{1}{2} \frac{d \overline{\Delta x^2}(\tau)}{d\tau} .
$$
${ D(\tau) }$ is called the (time-dependent) diffusion coefficient of the
particle. For the simplest Brownian particle of mass $m$ and mobility $\mu$
at temperature $T$, the diffusion coefficient is known to be
$$
    D(\tau)
    =
    \mu k_\mathrm{B} T
    \left(
        1 - \exp\left( -\frac{t}{m \mu} \right)
    \right) .
$$


## Velocity autocorrelation function

Another characteristic function of the stochastic dynamics is the velocity
autocorrelation function (VACF):
$$
    C(\tau)
    =
    \left\langle
      v(\tau) - v(0)
    \right\rangle
$$
where $v(t)$ is the velocity of the particle, i.e., $v(t) = dx(t)/dt$. It
represents how long the particle *remembers* the memory of its own trajectory.

Interestingly, VACF is related to diffusion coefficient in the following way:
$$
    D(\tau) = \int_0^\tau C(t) \,dt ;
$$
or, equivalently, ${ C(\tau) = dD(\tau)/d\tau }$.

