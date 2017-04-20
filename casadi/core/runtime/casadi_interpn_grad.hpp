template<typename T1>
void CASADI_PREFIX(interpn_grad)(T1* grad, int ndim, const T1* grid, const int* offset, const T1* values, const T1* x, const int* lookup_mode, int* iw, T1* w) {
  /* Quick return */
  if (!grad) return;
  /* Work vectors */
  T1* alpha = w; w += ndim;
  T1* coeff = w; w += ndim;
  int* index = iw; iw += ndim;
  int* corner = iw; iw += ndim;
  /* Left index and fraction of interval */
  CASADI_PREFIX(interpn_weights)(ndim, grid, offset, x, alpha, index, lookup_mode);
  /* Loop over all corners, add contribution to output */
  CASADI_PREFIX(fill_int)(corner, ndim, 0);
  CASADI_PREFIX(fill)(grad, ndim, 0.);
  do {
    /* Get coefficients */
    T1 v = CASADI_PREFIX(interpn_interpolate)(ndim, offset, values,
      alpha, index, corner, coeff);
    /* Propagate to alpha */
    int i;
    for (i=ndim-1; i>=0; --i) {
      if (corner[i]) {
        grad[i] += v*coeff[i];
        v *= alpha[i];
      } else {
        grad[i] -= v*coeff[i];
        v *= 1-alpha[i];
      }
    }
  } while (CASADI_PREFIX(flip)(corner, ndim));
  /* Propagate to x */
  int i;
  for (i=0; i<ndim; ++i) {
    const T1* g = grid + offset[i];
    int j = index[i];
    grad[i] /= g[j+1]-g[j];
  }
}