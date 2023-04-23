# urbit-cob

A C implementation of [urbit-ob](https://github.com/urbit/urbit-ob),
with much glancing at [urbit-pob](https://github.com/evening/urbit-pob).

### Build & install

There is the simplest-possible Makefile and a Nix flake.

### Use

The library is based on [GNU MP](https://gmplib.org/), making it
bignum-correct (as long as your host is 64-bit, otherwise most
probably not, sorry, testing & PRs welcome!) but rather awkward to
use.

The library exposes two families of functions:

* `num2patp / patp2num / patp2hex / is_valid_patp`
* `num2patq / patq2num / patq2hex / is_valid_patq`

As well as:

* `clan`, for determining the ship class of a (numeric) point
* `sein`, for determining the parent of a (numeric) point
* `patq_eq`, for comparing `@q` values for equality
