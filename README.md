# Database-2020

An in-memory database system with a SQL-like query parser, implemented in C++14. The database uses multiple advanced data structures for efficient indexing and querying on different fields. Every structure had been written from scratch.

## Overview

This project implements a hierarchical database system for storing records containing three fields:
- **name** (string) — person's name
- **phone** (integer) — phone number  
- **group** (integer) — group identifier

The system supports SQL-like operations: `SELECT`, `INSERT`, `DELETE` with `WHERE` clauses and various comparison operators.

## Data Structures

The database employs a multi-layered indexing architecture combining several data structures:

### 1. AVL Tree (`avl_tree.h`)

A self-balancing binary search tree implementation.

```
tree_node:
  ├── left (tree_node*)
  ├── right (tree_node*)  
  ├── height (int)
  └── data (list_node*)
```

### 2. Doubly Linked List (`linked_list.h`)

A generic doubly linked list used as the primary data container with **O(1)** insertion at head.

```
list_node (extends record):
  ├── next (list_node*)
  ├── prev (list_node*)
  ├── name (char*)
  ├── phone (int)
  └── group (int)
```

### 3. Hash Table (`database_index.h`)

An array of pointer lists implementing hash-based lookup by name:
- **Hash functions available**:
  - Basic: sum of character codes modulo p
  - DJB2 (enabled with `-DUSE_DJB2_HASH` flag): `hash = hash * 33 + c`
- **Collision handling**: chaining via `pointer_list`
- **Average time complexity**: O(1) for name lookups

```
pointer_list[p]:  // p = hash table size
  └── pointer_list_node chain
        ├── pointer (list_node*)
        ├── next
        └── prev
```

### 4. DatabaseIndex — Combined Index Structure

A composite data structure combining AVL tree and hash table:

```
database_index (record_index class):
  ├── tree tr          // AVL tree indexed by (phone, name)
  ├── pointer_list[]   // Hash table indexed by name
  ├── group (int)      // Group identifier for this database
  └── p (int)          // Hash table size
```

### 5. Group Index Tree (`database_index.h`)

An AVL tree where each node contains a complete `record_index` instance:
- **Key**: group number
- **Value**: `record_index` (tree + hash array) for records in that group
- Enables efficient queries filtered by group

```
group_index_node:
  ├── left (group_index_node*)
  ├── right (group_index_node*)
  ├── height (int)
  └── data (record_index*)   // Contains tree + hash for this group
```

### 6. Database — Top-Level Database (`database.h`)

The main database class orchestrating all structures:

```
database:
  ├── list record_list           // Primary container (all records)
  ├── group_index_tree group_index_tree // Index by group → (phone, name)
  └── record_index global_index  // Global index by (phone, name)
```

## Architecture Diagram

```
                            database
                               │
           ┌───────────────────┼───────────────────┐
           │                   │                   │
           ▼                   ▼                   ▼
         list          group_index_tree      record_index
      (container)         (by group)         (global index)
           │                   │                   │
           │           ┌───────┼───────┐           │
           │           ▼       ▼       ▼           │
           │         ri[g1]  ri[g2]  ri[g3]        │
           │           │                           │
           │     ┌─────┴─────┐              ┌──────┴──────┐
           │     ▼           ▼              ▼             ▼
           │  AVL tree   hash_table[]    AVL tree     hash_table[]
           │  (phone)      (name)        (phone)        (name)
           │
           ▼
    ┌──────┼──────┐
    │      │      │
  node1  node2  node3 ... (doubly linked)
```
where ri stands for record_index for phone and name of records from the same group.

## Query Parser (`parser.h`, `parser.cpp`)

The parser supports SQL-like syntax for database operations:

### Supported Commands

| Command | Syntax |
|---------|--------|
| SELECT | `select * [where <condition> [and <condition>]];` |
| INSERT | `insert (<name>, <phone>, <group>);` |
| DELETE | `delete [where <condition> [and <condition>]];` |
| QUIT | `quit;` |

### Condition Operators

| Operator | Description |
|----------|-------------|
| `=` | Equal |
| `<>` | Not equal |
| `<` | Less than |
| `>` | Greater than |
| `<=` | Less than or equal |
| `>=` | Greater than or equal |
| `like` | Pattern matching (supports `%` and `_` wildcards) |

### Example Queries

```sql
select *;
select * where phone = 12345;
select * where name = "John" and group = 5;
select * where name like "J%";
insert (Alice, 55555, 3);
delete where group = 1;
delete where phone = 12345 and name = "Bob";
quit;
```

## Building

### Requirements
- C++14 compatible compiler (g++ recommended)
- Make

### Compilation

```bash
make clean
make
```

### Compiler Flags

The Makefile supports different build configurations:

```makefile
# Basic build
CPPFLAGS = -std=c++14 -g -O3

# Enable DJB2 hash function (recommended for better distribution)
CPPFLAGS = -std=c++14 -O3 -Wall -Wextra -pedantic -DUSE_DJB2_HASH
```

The `-DUSE_DJB2_HASH` flag enables the optimized DJB2 hash function instead of the basic additive hash.

## Usage

```bash
./a.out <input_file> <hash_size>
```

**Parameters:**
- `<input_file>` — Text file containing initial records (format: `name phone group`)
- `<hash_size>` — Size of hash tables (integer > 0)

**Example:**
```bash
./a.out data.txt 1000
```

**Input file format:**
```
Alice 12345 1
Bob 23456 2
Charlie 8005553535 102
```

After loading, the program reads SQL-like commands from stdin.

## Tools

The `tools/` directory contains utility scripts for testing and development:

### Performance Testing
```bash
cd tools
./test_performance.sh ../data.txt ../queries.txt 1000
```

### Data Generation
```bash
cd tools
python3 generate_data.py
```

See `tools/README.md` for detailed documentation of available tools and usage examples.

## Performance Characteristics

| Operation | Time Complexity | Index Used |
|-----------|----------------|------------|
| Insert | O(log n) | All indices updated |
| Select by phone | O(log n) | AVL tree |
| Select by name (EQ) | O(1) avg | Hash table |
| Select by group | O(log g + log n) | Kilo tree → AVL |
| Select by name (LIKE) | O(n) | Full scan |
| Delete | O(log n) | All indices updated |

Where:
- n = total number of records
- g = number of distinct groups

## File Structure

```
Database-2020/
├── main.cpp             # Entry point, main loop
├── parser.h             # Parser declarations (record, command, parse)
├── parser.cpp           # Parser implementation
├── linked_list.h        # Doubly linked list + select/delete operations
├── avl_tree.h           # AVL tree implementation
├── database_index.h     # record_index, pointer_list, group_index_tree declarations
├── database_index.cpp   # record_index, group_index_tree implementations
├── database.h           # Top-level database with query dispatcher
├── Makefile             # Build configuration
├── README.md            # This file
└── tools/               # Utility scripts and tools
    ├── test_performance.sh    # Performance benchmarking script
    ├── generate_data.py       # Random data generator
    └── README.md             # Tools documentation
```

## Hash Function Analysis

The system includes a built-in hash quality analyzer (`how_good_is_hash()`):

```
./a.out data.txt 1000
```

### Performance Comparison (1M records)

| Hash Function | Flag | Query Time | Dispersion |
|---------------|------|------------|------------|
| Basic (sum) | none | ~505 sec | ~10^6 (poor) |
| DJB2 | `-DUSE_DJB2_HASH` | ~266 sec | ~10^3 (good) |

**Recommendation**: Always enable `-DUSE_DJB2_HASH` for production use (~2x faster).

## Author

Created in 2020 as a database systems project.

## License

Educational project — feel free to use for learning purposes.
