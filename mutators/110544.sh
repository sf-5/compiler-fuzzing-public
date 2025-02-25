#!/bin/bash

# Check if a file argument is provided
if [ $# -ne 2 ]; then
    echo "Usage: $0 <file> <seed>"
    exit 1
fi

file="$1"

sed -i -E "s/__attribute__\(\(optimize\(\"no\-tree\-vectorize\"\)\)\)//" "$file"