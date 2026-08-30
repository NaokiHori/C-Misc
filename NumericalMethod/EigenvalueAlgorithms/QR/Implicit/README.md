# Implicit QR algorithm (sequential deflation)

Implicit QR algorithm (with sequential deflation from the bottom)

## Schematic of bulge chasing

We consider `N = 6` system as an example.
The initial matrix is:

```
[ D  e  .  .  .  . ]
[ e  D  e  .  .  . ]
[ .  e  D  e  .  . ]
[ .  .  e  D  e  . ]
[ .  .  .  e  D  e ]
[ .  .  .  .  e  D ]
```

At k = 0, rotation angle is `(D[0] - shift, e[0])`, applied to the 0th and 1st rows / columns to get:

```
[ D  e  B  .  .  . ]
[ e  D  e  .  .  . ]
[ B  e  D  e  .  . ]
[ .  .  e  D  e  . ]
[ .  .  .  e  D  e ]
[ .  .  .  .  e  D ]
```

At k = 1, rotation angle is determined to eliminate B, applied to the 1st and 2nd rows / columns to get:

```
[ D  e  .  .  .  . ]
[ e  D  e  B  .  . ]
[ .  e  D  e  .  . ]
[ .  B  e  D  e  . ]
[ .  .  .  e  D  e ]
[ .  .  .  .  e  D ]
```

We repeat the same procedure up to k = 3 to get:

```
[ D  e  .  .  .  . ]
[ e  D  e  .  .  . ]
[ .  e  D  e  .  . ]
[ .  .  e  D  e  B ]
[ .  .  .  e  D  e ]
[ .  .  .  B  e  D ]
```

At k = 4, rotation angle is determined to eliminate B, applied to the 4th and 5th rows / columns to get:

```
[ D  e  .  .  .  . ]
[ e  D  e  .  .  . ]
[ .  e  D  e  .  . ]
[ .  .  e  D  e  . ]
[ .  .  .  e  D  e ]
[ .  .  .  .  e  D ]
```

## Deflation with bulge chasing

We consider the same system `N = 6`, whose starting (`n_start`) / ending (`n_end`) indices are `0` and `N`, respectively.
After some iterations, the system may lead to

```
[ D  e  .  .  .  . ]
[ e  D  e  .  .  . ]
[ .  e  D  .  .  . ]
[ .  .  .  D  e  . ]
[ .  .  .  e  D  e ]
[ .  .  .  .  e  D ]
```

Here `n = M = 2` (notice the index notation of sub-diagonal elements) has zero sub-diagonal item.
Now this system has two decoupled sub systems which can be treated independently:

- (n_start, n_end) = (0, M + 1)
- (n_start, n_end) = (M + 1, N)

## Reference

- [QR algorithm - Wikipedia](https://en.wikipedia.org/wiki/QR_algorithm)

