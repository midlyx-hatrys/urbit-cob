#include "ob.h"

#include <assert.h>
#include <murmur3.h>
#include <stdbool.h>

static uint32_t f(uint32_t j, uint32_t key) {
  const uint32_t raku[] = {0xB76D5EED, 0xEE281300, 0x85BCAE01, 0x4B387AF7};
  assert(j < sizeof raku);
  uint8_t bytes[2];
  bytes[0] = key & 0xFF;
  bytes[1] = (key & 0xFF00) / 0x100;
  uint32_t res = 0;
  MurmurHash3_x86_32(bytes, 2, raku[j], &res);
  return res;
}

typedef struct hilo {
  uint64_t hi, lo;
} hilo_t;

static void loop_hi_lo_init(hilo_t *hilo, uint64_t v) {
  hilo->lo = v & 0xFFFFFFFF;
  hilo->hi = v & 0xFFFFFFFF00000000;
}

static void fe_loop(mpz_t res, uint32_t r, uint32_t a, uint32_t b, uint32_t j, const mpz_t ell, const mpz_t arr) {
  if (j > r) {
    mpz_set_ui(res, a);
    if (r % 2 != 0 || mpz_cmp_ui(arr, a) == 0) {
      mpz_mul(res, res, arr);
      mpz_add(res, res, ell);
    } else {
      mpz_mul(res, res, ell);
      mpz_add(res, res, arr);
    }
    return;
  }
  uint32_t eff = f(j - 1, mpz_get_ui(arr));
  mpz_add_ui(res, ell, eff);
  uint32_t use_value = j % 2 == 0 ? b : a;
  mpz_mod_ui(res, res, use_value);
  mpz_t narr; mpz_init_set(narr, res);
  fe_loop(res, r, a, b, j + 1, arr, narr);
  mpz_clear(narr);
}

static void fe(mpz_t res, uint32_t r, uint32_t a, uint32_t b, const mpz_t m) {
  mpz_t div, mod; mpz_inits(div, mod, NULL);
  mpz_fdiv_qr_ui(div, mod, m, a);
  fe_loop(res, r, a, b, 1, mod, div);
  mpz_clears(div, mod, NULL);
}

static void feis(mpz_t res, const mpz_t m) {
  fe(res, 4, 65535, 65536, m);
  if (mpz_cmp_ui(res, 0xFFFFFFFF) >= 0) {
    mpz_t nm; mpz_init_set(nm, res);
    fe(res, 4, 65535, 65536, nm);
    mpz_clear(nm);
  }
}

static void fen_loop(mpz_t res, uint32_t a, uint32_t b, uint32_t j, const mpz_t ell, const mpz_t arr) {
  if (j < 1) {
    mpz_set_ui(res, a);
    mpz_mul(res, res, arr);
    mpz_add(res, res, ell);
    return;
  }
  uint32_t eff = f(j - 1, mpz_get_ui(ell));
  uint32_t use_value = j % 2 == 0 ? b : a;
  mpz_add_ui(res, arr, use_value);
  mpz_sub_ui(res, res, eff % use_value);
  mpz_mod_ui(res, res, use_value);
  mpz_t nell; mpz_init_set(nell, res);
  fen_loop(res, a, b, j - 1, nell, ell);
  mpz_clear(nell);
}

static void fen(mpz_t res, uint32_t r, uint32_t a, uint32_t b, const mpz_t m) {
  mpz_t ale, ahh; mpz_inits(ale, ahh, NULL);
  mpz_fdiv_qr_ui(ale, ahh, m, a);
  if (r % 2 == 1)
    mpz_swap(ale, ahh);
  fen_loop(res, a, b, r, ale, ahh);
  mpz_clears(ale, ahh, NULL);
}

static void tail(mpz_t res, const mpz_t v) {
  fen(res, 4, 65535, 65536, v);
  if (mpz_cmp_ui(res, 0xFFFFFFFF) >= 0) {
    mpz_t nm; mpz_init_set(nm, res);
    fen(res, 4, 65535, 65536, nm);
    mpz_clear(nm);
  }
}

void fein(mpz_t res, const mpz_t pyn) {
  hilo_t hilo;
  loop_hi_lo_init(&hilo, mpz_get_ui(pyn));
  if (mpz_cmp_ui(pyn, 0x10000) >= 0 && mpz_cmp_ui(pyn, 0xFFFFFFFF) <= 0) {
    mpz_t nm; mpz_init(nm);
    mpz_sub_ui(nm, pyn, 0x10000);
    feis(res, nm);
    mpz_clear(nm);
    mpz_add_ui(res, res, 0x10000);
  } else if (mpz_cmp_ui(pyn, 0x100000000) >= 0 && mpz_cmp_ui(pyn, 0xFFFFFFFFFFFFFFFF) <= 0) {
    mpz_t npyn; mpz_init(npyn);
    mpz_set_ui(npyn, hilo.lo);
    fein(res, npyn);
    mpz_clear(npyn);
    mpz_t bnhi; mpz_init_set_ui(bnhi, hilo.hi);
    mpz_ior(res, res, bnhi);
    mpz_clear(bnhi);
  } else {
    mpz_set(res, pyn);
  }
}

void fynd(mpz_t res, const mpz_t cry) {
  hilo_t hilo;
  loop_hi_lo_init(&hilo, mpz_get_ui(cry));
  if (mpz_cmp_ui(cry, 0x10000) >= 0 && mpz_cmp_ui(cry, 0xFFFFFFFF) <= 0) {
    mpz_t nv; mpz_init(nv);
    mpz_sub_ui(nv, cry, 0x10000);
    tail(res, nv);
    mpz_clear(nv);
    mpz_add_ui(res, res, 0x10000);
  } else if (mpz_cmp_ui(cry, 0x100000000) >= 0 && mpz_cmp_ui(cry, 0xFFFFFFFFFFFFFFFF) <= 0) {
    mpz_t ncry; mpz_init(ncry);
    mpz_set_ui(ncry, hilo.lo);
    fynd(res, ncry);
    mpz_clear(ncry);
    mpz_t bnhi; mpz_init_set_ui(bnhi, hilo.hi);
    mpz_ior(res, res, bnhi);
    mpz_clear(bnhi);
  } else {
    mpz_set(res, cry);
  }
}
