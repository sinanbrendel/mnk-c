{pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  packages = with pkgs; [
    clang-tools
    gnumake
    gdb
    gcc
    valgrind
  ];
}

