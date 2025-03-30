# Delaunay triangulation - naive method

From a given set of two-dimensional points, we perform the Delaunay triangulation.

## Caveat

To serve as a baseline, the most straightforward algorithm is adopted here: for each combination of three vertices, we check if the triangle is a valid piece.
The computational cost is `O(N^4)` (`N x NC3`) and it is not at all suited for practical use.

## Reference

- [Delaunay triangulation](https://en.wikipedia.org/wiki/Delaunay_triangulation#Algorithms)

