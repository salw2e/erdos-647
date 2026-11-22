# erdos-647

Computer search for [Erdős problem #647](https://www.erdosproblems.com/647): is there an n > 24 with

    max over m < n of (m + τ(m)) ≤ n + 2

where τ(m) is the number of divisors of m?

**There is none below 10^12.** This is an independent check of a bound that was already known: the same range was searched in June 2026 ([OEIS A087280](https://oeis.org/A087280)), and [arXiv 2608.17880](https://arxiv.org/abs/2608.17880) goes much further, to about 9.17 × 10^18, with a different method.

It also never comes close. For every n from 121 to 10^12 the max is at least n + 5. Every n where it is at most n + 4 is in [near-misses.txt](near-misses.txt), with the max minus n. The last one is n = 120.

## Run

    gcc -O3 -march=native -fopenmp -o search search.c
    ./search 1 1000000000000 4

It prints n and (max − n) for every n where that is at most the last argument. From 10^10 to 10^12 it took 6 hours on 16 threads of a Ryzen 9 7900.

## Check

`check.c` counts divisors a different way (prime factorization, not divisor pairs). Both print the same value for every n in windows near 10^6, 10^11, 10^12 and 10^13:

    gcc -O2 -o check check.c
    ./check 999999000000 1000001000000 100000 > a
    ./search 999999000000 1000001000000 100000 | sort -n > b
    cmp a b
