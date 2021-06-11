Implements [Gillespie's tau-leaping][Gsp01]:

    x(t + τ) = x(t) + poisson(kτ) v

where x is the number of molecules, k is the rate of a reaction, and v is the
change of x upon a single reaction. The program simulates a three-gene
repressilator.

[Gsp01]: https://aip.scitation.org/doi/abs/10.1063/1.1378322
