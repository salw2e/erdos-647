// Erdos problem 647: is there n > 24 with max_{m<n} (m + tau(m)) <= n + 2 ?
// usage: search FROM TO [SLACK]   prints every n in [FROM,TO) with max_{m<n}(m+tau(m)) - n <= SLACK
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define CHUNK (1ull << 24)
#define OV 65536 // tau(m) < OV for every m < 10^15, so m below lo-OV can never matter for n >= lo

typedef uint64_t u64;

int main(int argc, char **argv) {
  if (argc < 3) { fprintf(stderr, "usage: search FROM TO [SLACK]\n"); return 1; }
  u64 from = strtoull(argv[1], 0, 10), to = strtoull(argv[2], 0, 10);
  long slack = argc > 3 ? atol(argv[3]) : 2;
  if (from < 1) from = 1;
  if (to > 1000000000000000ull) { fprintf(stderr, "TO above 10^15 needs a bigger OV\n"); return 1; }
  u64 chunks = (to - from + CHUNK - 1) / CHUNK, done = 0;
  unsigned maxtau = 0;

  #pragma omp parallel
  {
    uint16_t *t = malloc((CHUNK + OV) * sizeof *t);
    #pragma omp for schedule(dynamic, 1) reduction(max:maxtau)
    for (u64 c = 0; c < chunks; c++) {
      u64 lo = from + c * CHUNK, hi = lo + CHUNK < to ? lo + CHUNK : to;
      u64 a = lo > OV ? lo - OV : 1, len = hi - a;
      for (u64 i = 0; i < len; i++) t[i] = 0;
      for (u64 d = 1; d * d < hi; d++) {
        u64 q = (a + d - 1) / d;
        if (q <= d) { if (d * d >= a) t[d * d - a] += 1; q = d + 1; }
        for (u64 m = q * d; m < hi; m += d) t[m - a] += 2;
      }
      u64 best = 0;
      for (u64 m = a; m < hi; m++) {
        if (m >= lo && best <= m + slack) {
          #pragma omp critical
          printf("%llu %lld\n", (unsigned long long)m, (long long)(best - m));
        }
        unsigned x = t[m - a];
        if (x > maxtau) maxtau = x;
        if (m + x > best) best = m + x;
      }
      #pragma omp atomic
      done++;
      if (done % 4096 == 0) {
        #pragma omp critical
        { fprintf(stderr, "%llu/%llu chunks\n", (unsigned long long)done, (unsigned long long)chunks); fflush(stdout); }
      }
    }
    free(t);
  }
  fprintf(stderr, "max tau seen %u\n", maxtau);
  if (maxtau >= OV) { fprintf(stderr, "OV too small\n"); return 2; }
  return 0;
}
