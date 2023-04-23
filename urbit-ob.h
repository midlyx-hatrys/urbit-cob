#pragma once

#include <gmp.h>
#include <stdbool.h>

typedef enum {
  GALAXY,
  STAR,
  PLANET,
  MOON,
  COMET,
  CLANLESS
} clan_t;

bool is_valid_patp(const char *arg);
char *num2patp(const mpz_t p);
bool patp2num(mpz_t res, const char *who);
char *patp2hex(const char *who);

bool parse_patp(mpz_t res, clan_t *clan, const char *who);

clan_t clan(const mpz_t p);
void sein(mpz_t res, const mpz_t p);
bool patq_eq(const char *a, const char *b);

bool is_valid_patq(const char *arg);
char *num2patq(const mpz_t v);
bool patq2num(mpz_t res, const char *who);
char *patq2hex(const char *who);
