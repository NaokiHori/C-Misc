# Discrete Fourier Transform

## Definition

The forward and backward transforms are defined as

```math
{{
  spectral(index1)
  =
  sum_between(
    index0,
    0,
    dof - 1,
    physical(index0) * twiddle(dof, index0, index1)
  )
}}
```

and

```math
{{
  physical(index0)
  =
  fraction(
    1,
    dof
  )
  *
  sum_between(
    index1,
    0,
    dof - 1,
    spectral(index1) * twiddle(dof, - index0, index1)
  )
}},
```

respectively, where ${{ index0 }}$ and ${{ index1 }}$ are integers $0, 1, \cdots, {{ dof - 1 }}$.

The twiddle factor is defined as

```math
{{
  twiddle(dof, index0, index1)
  =
  exponential(- imaginary_unit * fraction(2 * pi * index0 * index1, dof))
}}.
```

## Decimation in time

We consider an integer ${{ radix }}$ which satisfies

```math
\begin{aligned}
{{ dof }} & \equiv 0 \mod {{ radix }}, \\
{{ quotient }} & = {{ dof }} / {{ radix }}.
\end{aligned}
```

The forward transform leads to

```math
{{
  spectral(index1)
}}
=
{{
  sum_between(
    index2,
    0,
    radix - 1,
    sum_between(
      index0,
      0,
      dof / radix - 1,
      physical(radix * index0 + index2)
        * twiddle(dof, radix * index0 + index2, index1)
    )
  )
}}
=
{{
  sum_between(
    index2,
    0,
    radix - 1,
    twiddle(dof, index2, index1)
      * sum_between(
        index0,
        0,
        quotient - 1,
        physical(radix * index0 + index2)
          * twiddle(quotient, index0, index1)
      )
  )
}}.
```

By assigning ${{ index1 = quotient * index4 + index3 }}$, we obtain

```math
{{
  spectral(quotient * index4 + index3)
  =
  sum_between(
    index2,
    0,
    radix - 1,
    twiddle(radix, index2, index4)
      * twiddle(dof, index2, index3)
      * sum_between(
        index0,
        0,
        quotient - 1,
        physical(radix * index0 + index2)
          * twiddle(quotient, index0, index3)
      )
  )
}},
```

where ${{ index3 }} = 0, 1, \cdots, {{ quotient - 1 }}$ and ${{ index4 }} = 0, 1, \cdots, {{ radix - 1 }}$.

## Method

To enable parallel execution across threads, the algorithm avoids heap memory allocations at runtime.
To achieve this, it reorders the input buffer in place before performing the butterfly operations.

Consider, for example, a forward transform of size `12 = 2 * 2 * 3`:

```
DFT(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11).
```

Using the decimation in time defined earlier, this can be expressed in terms of three independent DFTs:

```
DFT(0, 3, 6, 9), DFT(1, 4, 7, 10), DFT(2, 5, 8, 11).
```

Since each of these DFTs has size `4 = 2 * 2`, it can be split further:

```
DFT(0, 6), DFT(3, 9), DFT(1, 7), DFT(4, 10), DFT(2, 8), DFT(5, 11),
```

or

```
DFT(0), DFT(6), DFT(3), DFT(9), DFT(1), DFT(7), DFT(4), DFT(10), DFT(2), DFT(8), DFT(5), DFT(11).
```

The butterfly operations for this can be executed in place.

Let us consider a concrete process for performing this reordering.
First, compute the target ordering (the reordered sequence above).
This can be done by recursively splitting by prime factors; for example, the code looks like this:

```python
def reorder(factors, values):
    if not factors:
        return values
    factor = factors[0]
    out = []
    for start in range(factor):
        out.extend(reorder(factors[1:], values[start::factor]))
    return out

reordered = reorder((3, 2, 2), [i for i in range(12)])
```

By itself, this is somewhat inconvenient for reordering actual inputs, so we want to structure it so that reordering is completed simply by applying a swap list to the input:

```python
def apply_swap_list(swap_list, values):
    for swap in swap_list:
        i, j = swap
        values[i], values[j] = values[j], values[i]
    return values
```

This can be obtained from the result of `reorder` as follows:

```python
def get_swap_list(reordered):
    nitems = len(reordered)
    swap_list = list()
    values = [i for i in range(nitems)]
    for i in range(nitems):
        if values[i] != reordered[i]:
            for j in range(i + 1, nitems):
                if values[j] == reordered[i]:
                    values[i], values[j] = values[j], values[i]
                    swap_list.append((i, j))
                    break
    return swap_list
```

Note that while the swap list must be applied on every execution, the operation is identical whenever the input size is fixed and the order of prime factors is predetermined.
Precomputing and storing it in a `plan` struct alongside the twiddle/trigonometric tables reduces overhead during DFT execution.

