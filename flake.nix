{
  description = "urbit-cob";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";

    parts = {
      url = "github:hercules-ci/flake-parts";
      inputs.nixpkgs-lib.follows = "nixpkgs";
    };

    mini-compile-commands = {
      url = "github:danielbarter/mini_compile_commands";
      flake = false;
    };
  };

  outputs = inputs @ {
    self
    , nixpkgs
    , parts
    , mini-compile-commands
  }: parts.lib.mkFlake { inherit inputs; } (let
    inherit (builtins) attrValues;

    pkgAttrs = pkgs: {
      pname = "urbit-cob";
      version = "0.1";
      src = ./.;
      outputs = ["out" "dev"];
      doCheck = true;
      nativeBuildInputs = attrValues {
        inherit (pkgs)
          gnumake
        ;
      };
      buildInputs = attrValues {
        inherit (pkgs)
          gmp
          mmh3
        ;
      };
      installFlags = ["PREFIX=$(out)"];
    };
  in {
    systems = ["x86_64-linux" "aarch64-linux" "aarch64-darwin" "x86_64-darwin"];

    flake = {
      overlays.default = pkgs: _: {
        urbit-cob = pkgs.stdenv.mkDerivation (pkgAttrs pkgs);

        # let's just package this for our own use
        mmh3 = pkgs.stdenv.mkDerivation {
          pname = "mmh3";
          version = "unstable-2015-05-02";
          src = pkgs.fetchFromGitHub {
            owner = "PeterScott";
            repo = "murmur3";
            rev = "dae94be0c0f54a399d23ea6cbe54bca5a4e93ce4";
            hash = "sha256-I7a5beN+4lle0CGz911PN56y+bKsJfQ7Kpim+L7PVZg=";
          };
          outputs = ["out" "dev"];
          doCheck = true;
          nativeBuildInputs = attrValues {
            inherit (pkgs)
              gnumake
            ;
          };
          buildPhase = ''
            gcc -O3 -Wall -o murmur3.o -c murmur3.c
            gcc -O3 -Wall -o test.o -c test.c
            ar r libmmh3.a murmur3.o
          '';
          checkPhase = ''
            gcc -o tests test.o murmur3.o
            ./tests
          '';
          installPhase = ''
            mkdir -p $out/include $out/lib
            cp libmmh3.a $out/lib/
            cp murmur3.h $out/include/
          '';
        };
      };
    };

    perSystem = { pkgs, system, ... }: {
      imports = [{ config._module.args.pkgs = import nixpkgs { inherit system; overlays = [self.overlays.default]; }; }];

      packages.default = pkgs.urbit-cob;

      devShells.default = let
        attrs = pkgAttrs pkgs;
      in (pkgs.mkShell.override { stdenv = (pkgs.callPackage mini-compile-commands { }).wrap pkgs.stdenv; })
        (attrs // {
          nativeBuildInputs = attrs.nativeBuildInputs ++ (attrValues {
            inherit (pkgs)
              gdb
              valgrind
            ;
          });
        });
    };
  });
}
