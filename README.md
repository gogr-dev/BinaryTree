# BinaryTree

A C++17 **binary search tree** with a small interactive app on top: it loads the 1,000
most common male baby names (name, count, percentage, rank) into a BST keyed by name and
lets you look them up, list them alphabetically, or remove them.

No dependencies — just a C++17 compiler and `make`.

## Usage

```bash
make            # build the `names` binary
make run        # build + launch the interactive lookup
make test       # build + run the test suite
```

```
$ ./names              # or: ./names path/to/data.txt
Loaded 1000 names from male.txt.
Type a name to look it up, or 'help' for commands.
> liam
  name:       Liam
  rank:       1
  count:      19837
  percentage: 1.33%
> list
Aarav        rank 561  count 488    0.03%
Aaron        rank 60   count 5953   0.4%
...
> quit
```

Lookups are case-insensitive (`liam`, `LIAM` and `Liam` all match). The prompt also
exits cleanly on end-of-input (Ctrl-D).

## The data structure

`BinaryTree` (in `tree.h` / `tree.cpp`) is a classic unbalanced BST ordered by name:

- `insert` — rejects duplicate names instead of silently double-inserting.
- `search` — returns `std::optional<NameRecord>`, so misses are explicit.
- `remove` — handles all three cases (leaf, one child, two children); the two-children
  case promotes the in-order successor's **entire record**.
- `inOrder` — returns all records alphabetically.
- Nodes are owned by `std::unique_ptr`, so destruction is automatic; the copy
  constructor and copy assignment do an explicit deep clone (rule of five).

The data file format is whitespace-separated rows: `Name Count Percentage Rank`.

## Tests

`make test` runs 41 assertions covering insert/search, duplicate rejection, sorted
traversal, all three removal cases, deep copy + assignment, and a full parse of the real
data file. Two of them are regression tests for bugs in the original implementation:

- **Two-children removal data corruption** — only the successor's *name* was copied into
  the deleted slot, pairing it with the deleted node's count/percentage/rank.
- **Lossy copies** — the copy helper dropped `Percentage` and `Rank`, and there was no
  copy-assignment operator at all (double-free risk under the rule of three).

Other fixes along the way: `while (!eof())` read loops replaced with stream-state loops
(the original interactive loop spun forever on Ctrl-D), include guards, no
`using namespace std` in the header, `nullptr`/`unique_ptr` instead of `NULL`/raw
pointers, and the CLI separated from the data structure (`main.cpp` vs `tree.cpp`).

## Project structure

```
tree.h               # BinaryTree interface + NameRecord
tree.cpp             # implementation
main.cpp             # interactive CLI
tests/test_tree.cpp  # assertion-based test suite
male.txt             # dataset: 1000 names, one "Name Count Percentage Rank" per line
Makefile
```

## License

MIT.
