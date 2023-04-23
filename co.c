#include "urbit-ob.h"
#include "ob.h"

#include <assert.h>
#include <gmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char PREFIXES[] = ""
"doz\0mar\0bin\0wan\0sam\0lit\0sig\0hid\0fid\0lis\0sog\0dir\0wac\0sab\0wis\0sib\0"
"rig\0sol\0dop\0mod\0fog\0lid\0hop\0dar\0dor\0lor\0hod\0fol\0rin\0tog\0sil\0mir\0"
"hol\0pas\0lac\0rov\0liv\0dal\0sat\0lib\0tab\0han\0tic\0pid\0tor\0bol\0fos\0dot\0"
"los\0dil\0for\0pil\0ram\0tir\0win\0tad\0bic\0dif\0roc\0wid\0bis\0das\0mid\0lop\0"
"ril\0nar\0dap\0mol\0san\0loc\0nov\0sit\0nid\0tip\0sic\0rop\0wit\0nat\0pan\0min\0"
"rit\0pod\0mot\0tam\0tol\0sav\0pos\0nap\0nop\0som\0fin\0fon\0ban\0mor\0wor\0sip\0"
"ron\0nor\0bot\0wic\0soc\0wat\0dol\0mag\0pic\0dav\0bid\0bal\0tim\0tas\0mal\0lig\0"
"siv\0tag\0pad\0sal\0div\0dac\0tan\0sid\0fab\0tar\0mon\0ran\0nis\0wol\0mis\0pal\0"
"las\0dis\0map\0rab\0tob\0rol\0lat\0lon\0nod\0nav\0fig\0nom\0nib\0pag\0sop\0ral\0"
"bil\0had\0doc\0rid\0moc\0pac\0rav\0rip\0fal\0tod\0til\0tin\0hap\0mic\0fan\0pat\0"
"tac\0lab\0mog\0sim\0son\0pin\0lom\0ric\0tap\0fir\0has\0bos\0bat\0poc\0hac\0tid\0"
"hav\0sap\0lin\0dib\0hos\0dab\0bit\0bar\0rac\0par\0lod\0dos\0bor\0toc\0hil\0mac\0"
"tom\0dig\0fil\0fas\0mit\0hob\0har\0mig\0hin\0rad\0mas\0hal\0rag\0lag\0fad\0top\0"
"mop\0hab\0nil\0nos\0mil\0fop\0fam\0dat\0nol\0din\0hat\0nac\0ris\0fot\0rib\0hoc\0"
"nim\0lar\0fit\0wal\0rap\0sar\0nal\0mos\0lan\0don\0dan\0lad\0dov\0riv\0bac\0pol\0"
"lap\0tal\0pit\0nam\0bon\0ros\0ton\0fod\0pon\0sov\0noc\0sor\0lav\0mat\0mip\0fip\0"
"";
static const char *prefix(uint8_t i) {
  return PREFIXES + i * 4;
}

const char SUFFIXES[] = ""
"zod\0nec\0bud\0wes\0sev\0per\0sut\0let\0ful\0pen\0syt\0dur\0wep\0ser\0wyl\0sun\0"
"ryp\0syx\0dyr\0nup\0heb\0peg\0lup\0dep\0dys\0put\0lug\0hec\0ryt\0tyv\0syd\0nex\0"
"lun\0mep\0lut\0sep\0pes\0del\0sul\0ped\0tem\0led\0tul\0met\0wen\0byn\0hex\0feb\0"
"pyl\0dul\0het\0mev\0rut\0tyl\0wyd\0tep\0bes\0dex\0sef\0wyc\0bur\0der\0nep\0pur\0"
"rys\0reb\0den\0nut\0sub\0pet\0rul\0syn\0reg\0tyd\0sup\0sem\0wyn\0rec\0meg\0net\0"
"sec\0mul\0nym\0tev\0web\0sum\0mut\0nyx\0rex\0teb\0fus\0hep\0ben\0mus\0wyx\0sym\0"
"sel\0ruc\0dec\0wex\0syr\0wet\0dyl\0myn\0mes\0det\0bet\0bel\0tux\0tug\0myr\0pel\0"
"syp\0ter\0meb\0set\0dut\0deg\0tex\0sur\0fel\0tud\0nux\0rux\0ren\0wyt\0nub\0med\0"
"lyt\0dus\0neb\0rum\0tyn\0seg\0lyx\0pun\0res\0red\0fun\0rev\0ref\0mec\0ted\0rus\0"
"bex\0leb\0dux\0ryn\0num\0pyx\0ryg\0ryx\0fep\0tyr\0tus\0tyc\0leg\0nem\0fer\0mer\0"
"ten\0lus\0nus\0syl\0tec\0mex\0pub\0rym\0tuc\0fyl\0lep\0deb\0ber\0mug\0hut\0tun\0"
"byl\0sud\0pem\0dev\0lur\0def\0bus\0bep\0run\0mel\0pex\0dyt\0byt\0typ\0lev\0myl\0"
"wed\0duc\0fur\0fex\0nul\0luc\0len\0ner\0lex\0rup\0ned\0lec\0ryd\0lyd\0fen\0wel\0"
"nyd\0hus\0rel\0rud\0nes\0hes\0fet\0des\0ret\0dun\0ler\0nyr\0seb\0hul\0ryl\0lud\0"
"rem\0lys\0fyn\0wer\0ryc\0sug\0nys\0nyl\0lyn\0dyn\0dem\0lux\0fed\0sed\0bec\0mun\0"
"lyr\0tes\0mud\0nyt\0byr\0sen\0weg\0fyr\0mur\0tel\0rep\0teg\0pec\0nel\0nev\0fes\0"
"";
static const char *suffix(uint8_t i) {
  return SUFFIXES + i * 4;
}

static int find_syl(const char *syl) {
  int i;
  for (i = 0; i < 256; ++i) {
    if (strncmp(syl, PREFIXES + i * 4, 3) == 0)
      return i;
    if (strncmp(syl, SUFFIXES + i * 4, 3) == 0)
      return i;
  }
  return -1;
}

#define CACHE_MPZ(n)                            \
  static mpz_t cached##n;                       \
  static void ensure##n(void) {                 \
    static bool initialized = false;            \
    if (!initialized) {                         \
      mpz_init_set_ui(cached##n, n);            \
      initialized = true;                       \
    }                                           \
  }
CACHE_MPZ(0)
CACHE_MPZ(2)
#define CACHED_MPZ(n) (ensure##n(), cached##n)

static void bex(mpz_t res, unsigned long n) {
  mpz_pow_ui(res, CACHED_MPZ(2), n);
}

static void bex_bex_mul(mpz_t res, unsigned long a, unsigned long b) {
  bex(res, a);
  mpz_mul_ui(res, res, b);
  assert(mpz_fits_ulong_p(res));
  bex(res, mpz_get_ui(res));
}

static void rsh(mpz_t res, unsigned long a, unsigned long b, const mpz_t c) {
  bex_bex_mul(res, a, b);
  mpz_div(res, c, res);
}

static unsigned long met(unsigned long a, const mpz_t b) {
  unsigned long c = 0;
  mpz_t tmp, nc;
  mpz_init_set(tmp, b);
  mpz_init(nc);
  while (mpz_cmp_ui(tmp, 0) > 0) {
    c++;
    mpz_set(nc, tmp);
    rsh(tmp, a, 1, nc);
  }
  mpz_clears(tmp, nc, NULL);
  return c;
}

static void end(mpz_t res, unsigned long a, unsigned long b, const mpz_t c) {
  bex_bex_mul(res, a, b);
  mpz_mod(res, c, res);
}

typedef bool (syl_cb_t)(const char *syl, uint8_t byte, void *data);

static bool for_syls(const char *pat, syl_cb_t cb, void *data) {
  if (pat[0] != '~')
    return false;
  pat++;
  char syl[4] = {0};
  size_t syl_i = 0;
  size_t n_syls = 0;
  char c;
  while ((c = *pat++)) {
    if (c == '-')
      continue;
    syl[syl_i++] = c;
    if (syl_i == 3) {
      int si = find_syl(syl);
      if (si < 0)
        return false;
      if (!cb(syl, (uint8_t)si, data))
        return false;
      syl_i = 0;
      n_syls++;
    }
  }
  return syl_i == 0 && n_syls > 0;
}

bool is_valid_patp(const char *arg) {
  mpz_t n; mpz_init(n);
  char *ref = NULL;
  bool ret = patp2num(n, arg) && (ref = num2patp(n)) && strcmp(ref, arg) == 0;
  free(ref);
  mpz_clear(n);
  return ret;
}

bool patq_eq(const char *a, const char *b) {
  char *ha = NULL;
  char *hb = NULL;
  bool res = false;
  if (!(ha = patq2hex(a)) || !(hb = patq2hex(b)))
    goto out;
  char *haa = ha;
  while (*haa == '0')
    haa++;
  char *hba = hb;
  while (*hba == '0')
    hba++;
  res = strcmp(haa, hba) == 0;
 out:
  free(hb);
  free(ha);
  return res;
}

bool is_valid_patq(const char *arg) {
  mpz_t n; mpz_init(n);
  char *ref = NULL;
  bool ret = patq2num(n, arg) && (ref = num2patq(n)) && patq_eq(ref, arg);
  free(ref);
  mpz_clear(n);
  return ret;
}

static char *patp_build(size_t dyy, const mpz_t v) {
  size_t buf_size = 8 * dyy + 2; // a tad wasteful
  char *buf = calloc(buf_size, 1);
  if (!buf)
    return NULL;
  size_t i = buf_size - 1;
  mpz_t bnlog, tv, ntv; mpz_inits(bnlog, tv, ntv, NULL);
  mpz_set(tv, v);
  size_t timp;
  for (timp = 0; timp < dyy; timp++) {
    end(bnlog, 4, 1, tv);
    mpz_set(ntv, tv);
    rsh(tv, 4, 1, ntv);
    unsigned long log = mpz_get_ui(bnlog) & 0xFFFF;
    const char *pre = prefix(log >> 8);
    const char *suf = suffix(log & 0xFF);
    const char *etc = timp % 4 != 0 ? "-" : (timp != 0 ? "--" : "");
    size_t chunk_size = 6 + strlen(etc);
    assert(i > chunk_size);
    i -= chunk_size;
    memcpy(buf + i, pre, 3);
    memcpy(buf + i + 3, suf, 3);
    memcpy(buf + i + 6, etc, strlen(etc));
  }
  buf[0] = '~';
  memmove(buf + 1, buf + i, buf_size - i);
  buf[buf_size - i] = '\0';
  mpz_clears(bnlog, tv, ntv, NULL);
  return buf;
}

char *num2patp(const mpz_t p) {
  char *res = NULL;
  mpz_t sxz; mpz_init(sxz);
  fein(sxz, p);
  size_t dyy = met(4, sxz);
  size_t dyx = met(3, sxz);
  if (dyx <= 1) {
    res = calloc(5, 1);
    if (!res)
      goto out;
    res[0] = '~';
    strcpy(res + 1, suffix(mpz_get_ui(sxz)));
  } else {
    res = patp_build(dyy, sxz);
  }
 out:
  mpz_clear(sxz);
  return res;
}

typedef struct {
  char *buf;
  size_t buf_size;
  size_t i;
} syl2hex_data_t;
static bool syl2hex_cb(const char *syl, uint8_t value, void *data_) {
  syl2hex_data_t *data = data_;
  if (data->i < (data->buf_size - 3)) {
    data->buf_size *= 2;
    data->buf = realloc(data->buf, data->buf_size);
    if (!data->buf)
      return false;
  }
  sprintf(data->buf + data->i, "%02x", (unsigned int)value);
  data->i += 2;
  return true;
}
static char *pat2hex(const char *pat) {
  const size_t initial_buf_size = 33; // enough for a comet
  syl2hex_data_t data = {
    .buf_size = initial_buf_size,
    .buf = calloc(initial_buf_size, 1),
    .i = 0
  };
  if (!data.buf)
    return NULL;
  if (!for_syls(pat, syl2hex_cb, &data)) {
    free(data.buf);
    return NULL;
  }
  return data.buf;
}

clan_t clan(const mpz_t p) {
  size_t wyd = met(3, p);
  if (wyd <= 1)
    return GALAXY;
  if (wyd <= 2)
    return STAR;
  if (wyd <= 4)
    return PLANET;
  if (wyd <= 8)
    return MOON;
  if (wyd <= 16)
    return COMET;
  return CLANLESS;
}

char *patp2hex(const char *who) {
  char *hex = NULL;
  mpz_t fy, cry; mpz_inits(fy, cry, NULL);
  char *res = NULL;
  if (!(hex = pat2hex(who)))
    goto out;
  mpz_set_str(cry, hex, 16);
  fynd(fy, cry);
  size_t hex_size = mpz_sizeinbase(fy, 16);
  size_t padding = hex_size % 2;
  if (!(res = malloc(mpz_sizeinbase(fy, 16) + 2 + padding)))
    goto out;
  res[0] = '0';
  mpz_get_str(res + padding, 16, fy);
 out:
  mpz_clears(fy, cry, NULL);
  free(hex);
  return res;
}

bool patp2num(mpz_t res, const char *who) {
  return parse_patp(res, NULL, who);
}

bool parse_patp(mpz_t res, clan_t *c, const char *who) {
  char *hex = patp2hex(who);
  if (!hex)
    return false;
  mpz_set_str(res, hex, 16);
  free(hex);
  if (c)
    *c = clan(res);
  return true;
}

static size_t cpair2num(const char *pair) {
  const char s[] = {pair[0], pair[1], '\0'};
  return strtoul(s, NULL, 16);
}

char *num2patq(const mpz_t v) {
  char *res = NULL;
  char *hexstr = mpz_get_str(NULL, 16, v);
  if (!hexstr)
    return NULL;
  if (strlen(hexstr) <= 2) {
    if (!(res = calloc(5, 1)))
      goto out;
    res[0] = '~';
    strcpy(res + 1, suffix(strtoul(hexstr, NULL, 16)));
  } else {
    size_t n_chunks = (strlen(hexstr) + 3) / 4;
    size_t buf_size = n_chunks * 7 + 2;
    if (!(res = calloc(buf_size, 1)))
      goto out;
    res[0] = '~';
    size_t mod = strlen(hexstr) % 4;
    size_t ri, hi;
    for (ri = 1, hi = 0; hi < strlen(hexstr); ri += 6) {
      char chunk[4];
      if (hi == 0 && mod != 0) {
        size_t i;
        for (i = 0; i < 4 - mod; i++)
          chunk[i] = '0';
        memcpy(chunk + 4 - mod, hexstr, mod);
        hi += mod;
      } else {
        memcpy(chunk, hexstr + hi, 4);
        hi += 4;
      }
      if (ri != 1)
        res[ri++] = '-';
      strcpy(res + ri, prefix(cpair2num(chunk)));
      strcpy(res + ri + 3, suffix(cpair2num(chunk + 2)));
    }
    assert(ri <= buf_size);
  }
 out:
  free(hexstr);
  return res;
}

char *patq2hex(const char *who) {
  return pat2hex(who);
}

bool patq2num(mpz_t res, const char *who) {
  char *hex = patq2hex(who);
  if (!hex)
    return false;
  mpz_set_str(res, hex, 16);
  free(hex);
  return true;
}

void sein(mpz_t res, const mpz_t p) {
  switch (clan(p)) {
    case GALAXY:
      mpz_set(res, p);
      break;
    case STAR:
      end(res, 3, 1, p);
      break;
    case PLANET:
      end(res, 4, 1, p);
      break;
    case MOON:
      end(res, 5, 1, p);
      break;
    case COMET:
    case CLANLESS:
      mpz_set(res, CACHED_MPZ(0));
      break;
  }
}
