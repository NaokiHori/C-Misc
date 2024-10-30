# Binary Search

A binary search implementation for arbitrary data types.

## Overview

The main function in this library performs a [binary search](https://en.wikipedia.org/wiki/Binary_search) on a sorted list of elements.  
Specifically, it finds the maximum index of an element that is less than or equal to the target value (known as [approximate matches](https://en.wikipedia.org/wiki/Binary_search#Approximate_matches)).  
If the target value is smaller than the smallest element in the list, the function returns `-1`.

For example, consider this sorted list of integers:

```
[2, 3, 5, 8, 13].
```

Below is a table showing the corresponding index for different target values:

| Target Range    | Returned Index |
|-----------------|----------------|
| `[INT_MIN, 1]`  | -1             |
| `2`             |  0             |
| `[3, 4]`        |  1             |
| `[5, 7]`        |  2             |
| `[8, 12]`       |  3             |
| `[13, INT_MAX]` |  4             |
