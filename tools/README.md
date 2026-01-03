# Database Tools

This directory contains utility scripts for testing and generating data for the Database-2020 project.

## Files

### `test_performance.sh`
A bash script for benchmarking database performance.

**Usage:**
```bash
./test_performance.sh <data_file> <queries_file> <hash_size>
```

**Example:**
```bash
./test_performance.sh ../data.txt ../queries.txt 1000
```

**Features:**
- Measures total execution time for data loading and query processing
- Validates input files before running
- Provides detailed performance reports

### `generate_data.py`
A Python script for generating random test data in the database format.

**Usage:**
```bash
python3 generate_data.py
```

**Features:**
- Generates random names (6-20 characters)
- Creates random 7-digit phone numbers
- Assigns random group IDs from predefined ranges (101-112, 201-212, 301-312, 401-412)
- Interactive prompts for filename and number of records

**Example Output:**
```
JohnDoe 1234567 301
AliceSmith 9876543 412
BobJohnson 5551234 201
```

## Usage Examples

### Generate Test Data
```bash
cd tools
python3 generate_data.py
# Follow prompts to create data.txt with 1000000 records
```

### Run Performance Tests
```bash
cd tools
./test_performance.sh ../data.txt ../queries.txt 1000
```

### Full Workflow
```bash
# Generate data
cd tools
python3 generate_data.py
# Enter: data.txt, 1000000

# Create queries file
echo -e "select * where group = 301;\nselect * where phone > 5000000;\nquit;" > ../queries.txt

# Run performance test
./test_performance.sh ../data.txt ../queries.txt 1000
```
