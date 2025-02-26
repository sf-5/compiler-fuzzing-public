#!/bin/bash

# Check if a file argument is provided
if [ $# -ne 2 ]; then
    echo "Usage: $0 <file> <seed>"
    exit 1
fi

file="$1"

sed -i -E "s/#pragma omp target map\(from : ([a-zA-Z_][a-zA-Z0-9_]*)\) num_threads\([0-9]+\)/#pragma omp target map\(from : \1\)/" "$file"