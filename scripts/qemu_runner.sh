#!/usr/bin/env bash

set -e

if [ $# -ne 2 ]; then
	echo "Usage: $0 <.elf>"
	exit 1
fi
