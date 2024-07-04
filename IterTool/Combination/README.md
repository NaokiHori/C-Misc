# Combinations

## Overview

Outputs all combinations of `nCr`; e.g., 

```
2 1 0
3 1 0
4 1 0
3 2 0
4 2 0
4 3 0
3 2 1
4 2 1
4 3 1
4 3 2
```

for `(n = 5, r = 3)`.

## Example

See `src/main.c`.

## Test

```shell
cc -DTEST_COMBINATIONS -Iinclude -std=c99 -Wall -Wextra -Werror src/combinations.c -o a.out
```

