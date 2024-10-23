#!/bin/bash

OS_TYPE=$(uname)

if [[ "$OS_TYPE" == "Linux" ]]; then
    echo "This is Linux"
    if command -v yum >/dev/null 2>&1; then
        echo "=> use yum as installer"
    elif command -v apt-get >/dev/null 2>&1; then
        echo "=> use apt-get as installer"
    elif command -v pacman >/dev/null 2>&1; then
        echo "=> use pacman as installer"
    else
        echo "=> unknown installer"
    fi
elif [[ "$OS_TYPE" == "Darwin" ]]; then
    echo "This is macOS"
elif [[ "$OS_TYPE" == "CYGWIN"* || "$OS_TYPE" == "MINGW"* ]]; then
    echo "This is Windows (via Cygwin or Git Bash)"
else
    echo "Unknown OS"
fi
