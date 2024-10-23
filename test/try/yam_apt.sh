#!/bin/bash

if command -v apt >/dev/null 2>&1; then
    echo "This system uses apt for package management."
    # Use apt to install packages
elif command -v yum >/dev/null 2>&1; then
    echo "This system uses yum for package management."
    # Use yum to install packages
else
    echo "Neither apt nor yum found. Unsupported package manager."
fi
