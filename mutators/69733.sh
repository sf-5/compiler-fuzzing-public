#!/bin/bash
if [ $# -ne 2 ]; then
    echo "Usage: $0 <file> <seed>"
    exit 1
fi


file="$1"

sed -i -E 's/single/parallel num\_threads\(1)/g' "$file"