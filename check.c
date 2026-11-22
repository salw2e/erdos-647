// independent check for search.c: tau by prime factorization instead of divisor pairs, one window, no threads
// usage: check FROM TO SLACK
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint64_t u64;
#define OV 65536
int main(int argc, char **argv) {
  u64 from = strtoull(argv[1], 0, 10), to = strtoull(argv[2], 0, 10); long slack = atol(argv[3]);
  u64 a = from > OV ? from - OV : 1, len = to - a, r = 1;
  while ((r + 1) * (r + 1) < to) r++;
  char *comp = calloc(r + 1, 1);
  u64 *rest = malloc(len * 8); unsigned *tau = malloc(len * 4);
  for (u64 i = 0; i < len; i++) rest[i] = a + i, tau[i] = 1;
  for (u64 p = 2; p <= r; p++) {
    if (comp[p]) continue;
    for (u64 k = p * p; k <= r; k += p) comp[k] = 1;
    for (u64 m = (a + p - 1) / p * p; m < to; m += p) {
      unsigned e = 0; u64 *x = &rest[m - a];
      while (*x % p == 0) *x /= p, e++;
      tau[m - a] *= e + 1;
    }
  }
  for (u64 i = 0; i < len; i++) if (rest[i] > 1) tau[i] *= 2;
  u64 best = 0;
  for (u64 m = a; m < to; m++) {
    if (m >= from && best <= m + slack) printf("%llu %lld\n", (unsigned long long)m, (long long)(best - m));
    if (m + tau[m - a] > best) best = m + tau[m - a];
  }
  return 0;
}
