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

## Reference

- [QR algorithm - Wikipedia](https://en.wikipedia.org/wiki/QR_algorithm)

