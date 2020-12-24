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
        \left( x(\tau) - x(0) \right)^2
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
      v(\tau) v(0)
    \right\rangle
$$
where $v(t)$ is the velocity of the particle, i.e., $v(t) = dx(t)/dt$. It
represents how long the particle *remembers* the memory of its own trajectory.

Interestingly, VACF is related to diffusion coefficient in the following way:
$$
    D(\tau) = \int_0^\tau C(t) \,dt ;
$$
or, equivalently, ${ C(\tau) = dD(\tau)/d\tau }$. This relation is relatively
easy to show. Firstly, note:
$$
    x(\tau) - x(0) = \int_0^\tau v(t) \, dt .
$$
Injecting this into the definition of MSD gives
$$
\begin{aligned}
    D(\tau)
    & =
        \frac{1}{2} \frac{d}{d\tau}
        \left\langle
            \left( x(\tau) - x(0) \right)^2
        \right\rangle
    \\
    & =
        \frac{1}{2} \frac{d}{d\tau}
        \left\langle
            \left( \int_0^\tau v(t) \, dt \right)^2
        \right\rangle
    \\
    & =
        \frac{1}{2}
        \left\langle
            \frac{d}{d\tau}
            \left( \int_0^\tau v(t) \, dt \right)^2
        \right\rangle
    \\
    & =
        \left\langle
            v(\tau)
            \int_0^\tau v(t) \, dt
        \right\rangle
    \\
    & =
        \int_0^\tau
        \left\langle
            v(\tau)
            v(t)
        \right\rangle
        dt .
\end{aligned}
$$
Now, by time shift invariance, we may shift time uniformly by $-t$ in the
bracket. So, we get
$$
\begin{aligned}
    D(\tau)
    & =
        \int_0^\tau
        \left\langle
            v(\tau - t)
            v(0)
        \right\rangle
        dt
    \\
    & =
        \int_0^\tau
        \left\langle
            v(t)
            v(0)
        \right\rangle
        dt
    \\
    & =
        \int_0^\tau C(t) \, dt .
\end{aligned}
$$

So, the second derivative of MSD is VACF!
$$
    \frac{d^2}{d\tau^2}
    \left\langle
        \left( x(\tau) - x(0) \right)^2
    \right\rangle
    =
    C(\tau) .
$$


## Potential hessian

The asymptotic behavior of short-time VACF ($\tau \to 0$) gives mechanistic
insight into the system. With Taylor expansion,
$$
\begin{aligned}
    C(\tau)
    & =
        \left\langle
            v(0)
            v(\tau)
        \right\rangle
    \\
    & =
        \left\langle
            v(-\tau / 2)
            v(\tau / 2)
        \right\rangle
    \\
    & \simeq
        \left\langle
            \left( v(0) - \frac{\tau}{2} v^\prime(0) \right)
            \left( v(0) + \frac{\tau}{2} v^\prime(0) \right)
        \right\rangle
    \\
    & =
        \left\langle
            { v(0) }^2
        \right\rangle
        -
        \frac{\tau^2}{4}
        \left\langle
            { v^\prime(0) }^2
        \right\rangle
        .
\end{aligned}
$$
If the population is the canonical ensemble, we have
$$
    \left\langle { v(0) }^2 \right\rangle = \frac{k_\mathrm{B} T}{2} .
$$
And, if the particle obey the Langevin dynamics
$$
    v^\prime(0)
    =
    -\frac{1}{m} \nabla V - \gamma v(0) + \sigma \,\frac{dW}{dt} ,
$$
...? Then?
