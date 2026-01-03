#!/bin/bash

# Database Performance Test Script
# Usage: ./test_performance.sh <data_file> <queries_file> <hash_size>

if [ $# -ne 3 ]; then
    echo "Usage: $0 <data_file> <queries_file> <hash_size>"
    echo "Example: $0 data.txt queries.txt 1000"
    exit 1
fi

DATA_FILE="$1"
QUERIES_FILE="$2"
HASH_SIZE="$3"

# Check if files exist
if [ ! -f "$DATA_FILE" ]; then
    echo "Error: Data file '$DATA_FILE' not found!"
    exit 1
fi

if [ ! -f "$QUERIES_FILE" ]; then
    echo "Error: Queries file '$QUERIES_FILE' not found!"
    exit 1
fi

# Check if executable exists
if [ ! -f "./a.out" ]; then
    echo "Error: Executable './a.out' not found! Run 'make' first."
    exit 1
fi

echo "=== Database Performance Test ==="
echo "Data file: $DATA_FILE"
echo "Queries file: $QUERIES_FILE"
echo "Hash size: $HASH_SIZE"
echo "================================="

# Run the performance test
echo "Running performance test..."
START_TIME=$(date +%s.%3N)

# Run the database with queries piped from the queries file
# The program reads from stdin, so we pipe the queries
cat "$QUERIES_FILE" | ./a.out "$DATA_FILE" "$HASH_SIZE"

END_TIME=$(date +%s.%3N)

# Calculate execution time
EXECUTION_TIME=$(echo "$END_TIME - $START_TIME" | bc -l)

echo "================================="
echo "Performance Test Complete"
echo "Total execution time: ${EXECUTION_TIME} seconds"
echo "================================="
