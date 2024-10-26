#!/bin/bash

OS_TYPE=$(uname)

if [[ "$OS_TYPE" == "Linux" ]]; then
    echo "This is Linux"
    if command -v yum >/dev/null 2>&1; then
        echo "=> use yum as installer"
    elif command -v apt-get >/dev/null 2>&1; then
        echo "=> use apt-get as installer"
        sudo apt install qemu gcc-riscv64-unknown-elf qemu-user-static gcc-riscv64-linux-gnu
    elif command -v pacman >/dev/null 2>&1; then
        echo "=> use pacman as installer"
    else
        echo "=> unknown installer"
    fi
elif [[ "$OS_TYPE" == "Darwin" ]]; then
    echo "This is macOS"
    # https://github.com/riscv-software-src/homebrew-riscv
    brew tap riscv-software-src/riscv
    brew install riscv-tools qemu # rsicv64-elf-gcc
    # https://formulae.brew.sh/formula/riscv64-elf-gcc
elif [[ "$OS_TYPE" == "CYGWIN"* || "$OS_TYPE" == "MINGW"* ]]; then
    echo "This is Windows (via Cygwin or Git Bash)"
else
    echo "Unknown OS"
fi
