#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static const bool debug = false;

static void * mymalloc(
    const size_t size
){
  if(0 == size){
    goto err_hndl;
  }
  void * ptr = malloc(size);
  if(NULL == ptr){
    goto err_hndl;
  }
  return ptr;
err_hndl:
  fprintf(stderr, "%zu byte malloc failed\n", size);
  exit(EXIT_FAILURE);
}

static void myfree(
    void * ptr
){
  free(ptr);
}

static int get_f(
    const size_t n,
    double * f0
){
  // compute coefficients of the n-th order Gauss-Legendre polynomial
  // GL(n, x) =
  //   + f0[    0] x^{0}
  //   + f0[    1] x^{1}
  //   + f0[    2] x^{2}
  //   + ...
  //   + f0[n - 1] x^{n-1}
  //   + f0[n    ] x^{n  }
  // to this end, a recurrence relation
  //   GL(n, x) =
  //     + (2 n - 1) / n * x * GL(n - 1, x)
  //     - (1 n - 1) / n     * GL(n - 2, x)
  //   GL(0, x) = 0 x + 1
  //   GL(1, x) = 1 x + 0
  // is used
  // ref: https://en.wikipedia.org/wiki/Legendre_polynomials#Recurrence_relations
  switch(n){
    case 0:
      {
        f0[0] = 1.;
        return 0;
      }
    case 1:
      {
        f0[0] = 0.;
        f0[1] = 1.;
        return 0;
      }
    default:
      {
        // initialise output
        for(size_t i = 0; i < n + 1; i++){
          f0[i] = 0.;
        }
        // (n-1)-th contribution
        {
          double * f1 = mymalloc((n    ) * sizeof(double));
          if(0 != get_f(n - 1, f1)){
            return 1;
          }
          for(size_t i = 1; i < n + 1; i++){
            f0[i] += (2. * n - 1.) / (1. * n) * f1[i - 1];
          }
          myfree(f1);
        }
        // (n-2)-th contribution
        {
          double * f2 = mymalloc((n - 1) * sizeof(double));
          if(0 != get_f(n - 2, f2)){
            return 1;
          }
          for(size_t i = 0; i < n - 1; i++){
            f0[i] -= (1. * n - 1.) / (1. * n) * f2[i    ];
          }
          myfree(f2);
        }
        return 0;
      }
  }
}

static int get_fp(
    const size_t n,
    const double * f,
    double * fp
){
  // compute fp: derivative of f
  //   by just computing (a_n x^n)' = a_n n x^{n-1}
  for(size_t i = 0; i < n; i++){
    fp[i] = f[i + 1] * (i + 1.);
  }
  return 0;
}

static double compute_fx(
    const size_t n,
    const double * f,
    const double x
){
  // return f(n, x): n-th order polynomial at x
  double val = 0.;
  double xi = 1.;
  for(size_t i = 0; i < n + 1; i++){
    val += f[i] * xi;
    xi *= x;
  }
  return val;
}

static int find_roots(
    const size_t n,
    const double * f,
    const double * fp,
    double * roots
){
    const double epsilon = 1.e-15;
    const size_t maxiter = 2 << n;
  // guess and set initial roots
  // NOTE: to avoid the neighbouring roots fall to the same point,
  //   each IC should be close enough to the final roots
  const double dx = 2. / (100. * n * n);
  double lx = -1.;
  double lval = compute_fx(n, f, lx);
  for(size_t i = 0; i < n; i++){
    while(1){
      const double rx = lx + dx;
      const double rval = compute_fx(n, f, rx);
      const bool sign_changed = lval * rval < 0.;
      lx = rx;
      lval = rval;
      if(sign_changed){
        roots[i] = lx;
        break;
      }
    }
  }
  // find roots
  // ref: http://www.kurims.kyoto-u.ac.jp/EMIS/journals/AMI/2006/barrera.pdf
  for(size_t i = 0; i < n; i++){
    for(size_t iter = 0; iter < maxiter; iter++){
      double root = roots[i];
      const double local_f  = compute_fx(n    , f , root);
      const double local_fp = compute_fx(n - 1, fp, root);
      double mul = 1.;
      for(size_t j = 0; j < i; j++){
        mul *= 1. / (root - roots[j]);
      }
      root -= local_f / (local_fp - local_f * mul);
      double residual = root - roots[i];
      residual = residual < 0. ? -1. * residual : residual;
      if(residual < epsilon || iter - 1 == maxiter){
        if(debug){
          printf("%5zu: % .15f in %5zu iterations (residual: % .1e)\n", i, root, iter, residual);
        }
        break;
      }
      roots[i] = root;
    }
  }
  return 0;
}

static int find_weights(
    const size_t n,
    const double * fp,
    const double * roots,
    double * weights
){
  for(size_t i = 0; i < n; i++){
    const double x = roots[i];
    const double y = compute_fx(n - 1, fp, x);
    weights[i] = 2. / (1. - (x * x)) / (y * y);
  }
  return 0;
}

static int print_vector(
    const char label[],
    const size_t n,
    const double * xs
){
  puts(label);
  for(size_t i = 0; i < n; i++){
    printf("\t%3zu % .15e\n", i, xs[i]);
  }
  return 0;
}

int main(
    void
){
  // compute roots and weights of N-th-order Gaussian quadrature
  const size_t n = 4;
  printf("%zu-th-order Gauss-Legendre polynomial\n", n);
  // allocate the following buffers
  //   1. coefficients of N-th order polynomial
  //   2. coefficients of its derivative
  //   3. roots of the Gauss-Legendre polynomial
  //   4. weights of the Gauss-Legendre polynomial
  double * f       = mymalloc((n + 1) * sizeof(double));
  double * fp      = mymalloc((n    ) * sizeof(double));
  double * roots   = mymalloc((n    ) * sizeof(double));
  double * weights = mymalloc((n    ) * sizeof(double));
  if(0 != get_f(n, f)){
    goto abort;
  }
  if(0 != get_fp(n, f, fp)){
    goto abort;
  }
  if(0 != find_roots(n, f, fp, roots)){
    goto abort;
  }
  if(0 != find_weights(n, fp, roots, weights)){
    goto abort;
  }
  if(debug){
    print_vector("coefficients of f", n + 1, f);
    print_vector("coefficients of fp", n, fp);
  }
  print_vector("roots", n, roots);
  print_vector("weights", n, weights);
abort:
  myfree(f);
  myfree(fp);
  myfree(roots);
  myfree(weights);
  return 0;
}

