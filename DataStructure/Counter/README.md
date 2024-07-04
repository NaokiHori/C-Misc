# Counter

A naive implementation of Python's `collections.Counter` using a hash map.

# Example

Assume we have a list:

```python
[0, 8, 2, 4, 8, 0, 8, 4, 3, 5, 5, 6, 3, 2, 8, 0]
```

The count of each element is as follows:

| 0 | 2 | 3 | 4 | 5 | 6 | 8 |
|---|---|---|---|---|---|---|
| 3 | 2 | 2 | 2 | 2 | 1 | 4 |

See `main.c`.

# Implementation

The table size is hard-coded in the code (by default `10007`), where each bucket has a naive singly-linked list.

