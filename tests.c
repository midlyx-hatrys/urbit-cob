#include "urbit-ob.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT(cond) assert_cond(__FILE__, __LINE__, cond, #cond)
void assert_cond(const char *file, int line, bool cond, const char *cond_str) {
  if (!cond) {
    fprintf(stderr, "%s:%d: condition failed: %s\n", file, line, cond_str);
    abort();
  }
}

#define ASSERT_PATP_EQ_STR(n, ref) assert_patp_eq_str(__FILE__, __LINE__, n, ref)
void assert_patp_eq_str(const char *file, int line, unsigned long n, const char *ref) {
  mpz_t ba; mpz_init_set_ui(ba, n);
  char *patp = num2patp(ba);
  if (strcmp(patp, ref)) {
    fprintf(stderr, "%s:%d: \"%s(patp(%lu))\" != \"%s\"\n", file, line, patp, n, ref);
    abort();
  }
  free(patp);
  mpz_clear(ba);
}

void test_patp(void) {
  ASSERT_PATP_EQ_STR(0, "~zod");
  ASSERT_PATP_EQ_STR(255, "~fes");
  ASSERT_PATP_EQ_STR(256, "~marzod");
  ASSERT_PATP_EQ_STR(65535, "~fipfes");
  ASSERT_PATP_EQ_STR(65536, "~dapnep-ronmyl");
  ASSERT_PATP_EQ_STR(14287616, "~rosmur-hobrem");
  ASSERT_PATP_EQ_STR(14287617, "~sallus-nodlut");
  ASSERT_PATP_EQ_STR(14287618, "~marder-mopdur");
  ASSERT_PATP_EQ_STR(14287619, "~laphec-savted");
  ASSERT_PATP_EQ_STR(4294967295, "~dostec-risfen");
  ASSERT_PATP_EQ_STR(4294967296, "~doznec-dozzod-dozzod");
}

#define ASSERT_PATQ_EQ_STR(n, ref) assert_patq_eq_str(__FILE__, __LINE__, n, ref)
void assert_patq_eq_str(const char *file, int line, unsigned long n, const char *ref) {
  mpz_t ba; mpz_init_set_ui(ba, n);
  char *patq = num2patq(ba);
  if (strcmp(patq, ref)) {
    fprintf(stderr, "%s:%d: \"%s(patq(%lu))\" != \"%s\"\n", file, line, patq, n, ref);
    abort();
  }
  free(patq);
  mpz_clear(ba);
}

void test_patq(void) {
  ASSERT_PATQ_EQ_STR(0, "~zod");
  ASSERT_PATQ_EQ_STR(255, "~fes");
  ASSERT_PATQ_EQ_STR(256, "~marzod");
  ASSERT_PATQ_EQ_STR(65535, "~fipfes");
  ASSERT_PATQ_EQ_STR(65536, "~doznec-dozzod");
  ASSERT_PATQ_EQ_STR(14287616, "~dozler-wanzod");
  ASSERT_PATQ_EQ_STR(14287617, "~dozler-wannec");
  ASSERT_PATQ_EQ_STR(14287618, "~dozler-wanbud");
  ASSERT_PATQ_EQ_STR(14287619, "~dozler-wanwes");
  ASSERT_PATQ_EQ_STR(4294967295, "~fipfes-fipfes");
  ASSERT_PATQ_EQ_STR(4294967296, "~doznec-dozzod-dozzod");
}

#define ASSERT_CLAN_EQ(patp, ref) assert_clan_eq(__FILE__, __LINE__, patp, ref)
void assert_clan_eq(const char *file, int line, const char *patp, clan_t ref) {
  mpz_t p; mpz_init(p);
  clan_t c;
  if (!parse_patp(p, &c, patp) || c != ref) {
    fprintf(stderr, "%s:%d: %d(clan(\"%s\")) != %d\n", file, line, c, patp, ref);
    abort();
  }
  mpz_clear(p);
}

void test_clan(void) {
  ASSERT_CLAN_EQ("~zod", GALAXY);
  ASSERT_CLAN_EQ("~fes", GALAXY);
  ASSERT_CLAN_EQ("~marzod", STAR);
  ASSERT_CLAN_EQ("~fipfes", STAR);
  ASSERT_CLAN_EQ("~dapnep-ronmyl", PLANET);
  ASSERT_CLAN_EQ("~rosmur-hobrem", PLANET);
  ASSERT_CLAN_EQ("~sallus-nodlut", PLANET);
  ASSERT_CLAN_EQ("~marder-mopdur", PLANET);
  ASSERT_CLAN_EQ("~laphec-savted", PLANET);
  ASSERT_CLAN_EQ("~dostec-risfen", PLANET);
  ASSERT_CLAN_EQ("~divrul-dalred-samhec-sidrex", MOON);
  ASSERT_CLAN_EQ("~dotmec-niblyd-tocdys-ravryg--panper-hilsug-nidnev-marzod", COMET);
}

#define ASSERT_SEIN_EQ(patp, ref) assert_sein_eq(__FILE__, __LINE__, patp, ref)
void assert_sein_eq(const char *file, int line, const char *patp, const char *ref) {
  mpz_t p, s, sp; mpz_inits(p, s, sp, NULL);
  if (!parse_patp(p, NULL, patp) || !parse_patp(s, NULL, ref)
      || (sein(sp, p), mpz_cmp(sp, s)) != 0) {
    fprintf(stderr, "%s:%d: sein(\"%s\") != \"%s\"\n", file, line, patp, ref);
    abort();
  }
  mpz_clears(p, s, sp, NULL);
}

void test_sein(void) {
  ASSERT_SEIN_EQ("~zod", "~zod");
  ASSERT_SEIN_EQ("~fes", "~fes");
  ASSERT_SEIN_EQ("~marzod", "~zod");
  ASSERT_SEIN_EQ("~fipfes", "~fes");
  ASSERT_SEIN_EQ("~dapnep-ronmyl", "~zod");
  ASSERT_SEIN_EQ("~rosmur-hobrem", "~wanzod");
  ASSERT_SEIN_EQ("~sallus-nodlut", "~wannec");
  ASSERT_SEIN_EQ("~marder-mopdur", "~wanbud");
  ASSERT_SEIN_EQ("~laphec-savted", "~wanwes");
  ASSERT_SEIN_EQ("~dostec-risfen", "~fipfes");
  ASSERT_SEIN_EQ("~divrul-dalred-samhec-sidrex", "~samhec-sidrex");
  ASSERT_SEIN_EQ("~dotmec-niblyd-tocdys-ravryg--panper-hilsug-nidnev-marzod", "~zod");
}

#define ASSERT_PATQ_EQ_NUM(patq, ref) assert_patq_eq_num(__FILE__, __LINE__, patq, ref)
void assert_patq_eq_num(const char *file, int line, const char *patq, unsigned long ref) {
  mpz_t n; mpz_init(n);
  if (!patq2num(n, patq) || mpz_get_ui(n) != ref) {
    fprintf(stderr, "%s:%d: %s(patq2num(\"%s\")) != %lu\n", file, line,
            mpz_get_str(NULL, 10, n), patq, ref);
    abort();
  }
  mpz_clear(n);
}

void test_patq_to_num(void) {
  ASSERT_PATQ_EQ_NUM("~zod", 0);
  ASSERT_PATQ_EQ_NUM("~fes", 255);
  ASSERT_PATQ_EQ_NUM("~marzod", 256);
  ASSERT_PATQ_EQ_NUM("~fipfes", 65535);
  ASSERT_PATQ_EQ_NUM("~doznec-dozzod", 65536);
  ASSERT_PATQ_EQ_NUM("~dozler-wanzod", 14287616);
  ASSERT_PATQ_EQ_NUM("~dozler-wannec", 14287617);
  ASSERT_PATQ_EQ_NUM("~dozler-wanbud", 14287618);
  ASSERT_PATQ_EQ_NUM("~dozler-wanwes", 14287619);
  ASSERT_PATQ_EQ_NUM("~fipfes-fipfes", 4294967295);
  ASSERT_PATQ_EQ_NUM("~doznec-dozzod-dozzod", 4294967296);
  ASSERT(!is_valid_patq("abcdefg"));
}

#define ASSERT_PATP_EQ_NUM(patp, ref) assert_patp_eq_num(__FILE__, __LINE__, patp, ref)
void assert_patp_eq_num(const char *file, int line, const char *patp, unsigned long ref) {
  mpz_t n; mpz_init(n);
  if (!patp2num(n, patp) || mpz_get_ui(n) != ref) {
    fprintf(stderr, "%s:%d: %s(patp2num(\"%s\")) != %lu\n", file, line,
            mpz_get_str(NULL, 10, n), patp, ref);
    abort();
  }
  mpz_clear(n);
}

void test_patp_to_num(void) {
  ASSERT_PATP_EQ_NUM("~zod", 0);
  ASSERT_PATP_EQ_NUM("~fes", 255);
  ASSERT_PATP_EQ_NUM("~marzod", 256);
  ASSERT_PATP_EQ_NUM("~fipfes", 65535);
  ASSERT_PATP_EQ_NUM("~dapnep-ronmyl", 65536);
  ASSERT_PATP_EQ_NUM("~rosmur-hobrem", 14287616);
  ASSERT_PATP_EQ_NUM("~sallus-nodlut", 14287617);
  ASSERT_PATP_EQ_NUM("~marder-mopdur", 14287618);
  ASSERT_PATP_EQ_NUM("~laphec-savted", 14287619);
  ASSERT_PATP_EQ_NUM("~dostec-risfen", 4294967295);
  ASSERT_PATP_EQ_NUM("~doznec-dozzod-dozzod", 4294967296);
  ASSERT(!is_valid_patp("abcdefg"));
}

#define ASSERT_PATP_EQ_HEX(patp, ref) assert_patp_eq_hex(__FILE__, __LINE__, patp, ref)
void assert_patp_eq_hex(const char *file, int line, const char *patp, const char *ref) {
  char *hex;
  if (!(hex = patp2hex(patp)) || strcmp(hex, ref)) {
    fprintf(stderr, "%s:%d: %s(patp2hex(\"%s\")) != \"%s\"\n", file, line,
            hex, patp, ref);
    abort();
  }
  free(hex);
}

void test_patp_to_hex(void) {
  ASSERT_PATP_EQ_HEX("~zod", "00");
  ASSERT_PATP_EQ_HEX("~fes", "ff");
  ASSERT_PATP_EQ_HEX("~marzod", "0100");
  ASSERT_PATP_EQ_HEX("~fipfes", "ffff");
  ASSERT_PATP_EQ_HEX("~dapnep-ronmyl", "010000");
  ASSERT_PATP_EQ_HEX("~rosmur-hobrem", "da0300");
  ASSERT_PATP_EQ_HEX("~sallus-nodlut", "da0301");
  ASSERT_PATP_EQ_HEX("~marder-mopdur", "da0302");
  ASSERT_PATP_EQ_HEX("~laphec-savted", "da0303");
  ASSERT_PATP_EQ_HEX("~dostec-risfen", "ffffffff");
  ASSERT_PATP_EQ_HEX("~doznec-dozzod-dozzod", "0100000000");
}

#define ASSERT_PATQ_EQ_HEX(patq, ref) assert_patq_eq_hex(__FILE__, __LINE__, patq, ref)
void assert_patq_eq_hex(const char *file, int line, const char *patq, const char *ref) {
  char *hex;
  if (!(hex = patq2hex(patq)) || strcmp(hex, ref)) {
    fprintf(stderr, "%s:%d: %s(patq2hex(\"%s\")) != \"%s\"\n", file, line,
            hex, patq, ref);
    abort();
  }
  free(hex);
}

void test_patq_to_hex(void) {
  ASSERT_PATQ_EQ_HEX("~zod", "00");
  ASSERT_PATQ_EQ_HEX("~fes", "ff");
  ASSERT_PATQ_EQ_HEX("~marzod", "0100");
  ASSERT_PATQ_EQ_HEX("~fipfes", "ffff");
  ASSERT_PATQ_EQ_HEX("~doznec-dozzod", "00010000");
  ASSERT_PATQ_EQ_HEX("~dozler-wanzod", "00da0300");
  ASSERT_PATQ_EQ_HEX("~dozler-wannec", "00da0301");
  ASSERT_PATQ_EQ_HEX("~dozler-wanbud", "00da0302");
  ASSERT_PATQ_EQ_HEX("~dozler-wanwes", "00da0303");
  ASSERT_PATQ_EQ_HEX("~fipfes-fipfes", "ffffffff");
  ASSERT_PATQ_EQ_HEX("~doznec-dozzod-dozzod", "000100000000");
}

void test_is_valid_patp(void) {
  ASSERT(is_valid_patp("~zod"));
  ASSERT(is_valid_patp("~marzod"));
  ASSERT(is_valid_patp("~nidsut-tomdun"));
  ASSERT(!is_valid_patp(""));
  ASSERT(!is_valid_patp("~"));
  ASSERT(!is_valid_patp("~hu"));
  ASSERT(!is_valid_patp("~what"));
  ASSERT(!is_valid_patp("sudnit-duntom"));
}

void test_is_valid_patq(void) {
  ASSERT(is_valid_patq("~zod"));
  ASSERT(is_valid_patq("~marzod"));
  ASSERT(is_valid_patq("~nidsut-tomdun"));
  ASSERT(is_valid_patq("~dozzod-binwes-nidsut-tomdun"));
  ASSERT(!is_valid_patq(""));
  ASSERT(!is_valid_patq("~"));
  ASSERT(!is_valid_patq("~hu"));
  ASSERT(!is_valid_patq("~what"));
  ASSERT(!is_valid_patq("sudnit-duntom"));
}

#define ASSERT_REVERSIBILITY(i) assert_reversibility(__FILE__, __LINE__, i)
void assert_reversibility(const char *file, int line, unsigned long i) {
  char *patp, *patq;
  mpz_t bi, r; mpz_inits(bi, r, NULL);
  mpz_set_ui(bi, i);
  if (!(patp = num2patp(bi)) || !patp2num(r, patp) || mpz_cmp(bi, r) != 0) {
    fprintf(stderr, "%s:%d: patp(%lx) fails to reverse\n", file, line, i);
    abort();
  }
  if (!(patq = num2patq(bi)) || !patq2num(r, patq) || mpz_cmp(bi, r) != 0) {
    fprintf(stderr, "%s:%d: patq(%lx) fails to reverse\n", file, line, i);
    abort();
  }
  mpz_clears(bi, r, NULL);
  free(patq);
  free(patp);
}

void test_reversibility(void) {
  unsigned long i;
  for (i = 0x9000; i < 0x12000; i += 7)
    ASSERT_REVERSIBILITY(i);
}

int main(int argc, const char *argv[]) {
  test_patp();
  test_patq();
  test_clan();
  test_sein();
  test_patp_to_num();
  test_patq_to_num();
  test_patp_to_hex();
  test_patq_to_hex();
  test_is_valid_patp();
  test_is_valid_patq();
  test_reversibility();
}
