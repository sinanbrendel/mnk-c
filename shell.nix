let
  pkgs = import <nixpkgs> {};
in
pkgs.mkShellNoCC {
  packages = with pkgs; [
    gnumake
    gdb
    gcc
    valgrind
    clang-tools
    # musl # compile with -static
  ];
}

