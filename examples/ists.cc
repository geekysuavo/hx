
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#include "hx/core.hh"

int main () {
  /* iters: number of iterations. */
  constexpr std::size_t iters = 500;

  /* m: number of measured points.
   * n: number of spectral points.
   */
  constexpr std::size_t m = 51;
  constexpr std::size_t n = 2048;

  /* mu: per-iteration threshold reduction factor. */
  constexpr double mu = 0.98;

  /* Y: measured data vector.
   * X: spectral vector.
   * S: schedule.
   */
  using Y = hx::array<hx::scalar<1>, m>;
  using X = hx::array<hx::scalar<1>, n>;
  using S = hx::schedule<m, n>;

  /* ids: schedule values. */
  constexpr X::index_type ids[m] = {
     0,   1,   2,   4,   5,   7,   8,  10,  12,  15,
    16,  18,  20,  24,  28,  32,  37,  45,  55,  57,
    68,  72,  81,  97, 115, 117, 137, 164, 186, 198,
   216, 234, 266, 309, 364, 407, 414, 456, 474, 509,
   594, 614, 647, 659, 666, 737, 850, 871, 872, 958,
   990
  };

  /* obs: measured values. */
  constexpr Y::base_type obs[m] = {
   {  1.000000000e+00, -2.143091427e-02 },
   {  7.832661920e-01,  1.454081512e-01 },
   {  2.695893279e-01,  1.763437709e-01 },
   { -4.551209150e-01, -7.293023398e-02 },
   { -3.334031214e-01, -2.116722723e-01 },
   {  2.473272228e-01, -2.922219461e-01 },
   {  2.758937564e-01, -2.112204300e-01 },
   { -3.654955560e-01, -1.663425615e-01 },
   { -5.896101670e-01, -1.335974658e-01 },
   {  2.096288342e-01,  1.277408443e-01 },
   {  2.349401330e-01,  1.408410961e-01 },
   { -4.458285639e-02,  1.378407868e-01 },
   { -7.616916420e-02,  4.291113483e-01 },
   {  8.161788740e-02, -1.501729539e-01 },
   {  2.715323258e-01, -2.315732582e-02 },
   { -1.024163342e-01, -4.585948903e-01 },
   { -1.804254396e-01,  5.741667622e-01 },
   {  1.858972031e-01,  4.820919361e-01 },
   {  1.166640912e-01, -2.376044976e-01 },
   { -1.860583210e-01, -2.683086618e-01 },
   { -1.365204060e-01,  7.806277463e-02 },
   {  1.160191447e-01,  1.215127243e-01 },
   { -2.112178929e-01,  2.060027241e-02 },
   {  8.118590290e-02,  3.788401171e-02 },
   { -3.588835282e-02,  3.041944021e-01 },
   { -8.980304851e-02,  5.513531626e-02 },
   { -1.913346807e-01,  1.653830939e-02 },
   { -4.304364248e-02, -1.568877584e-02 },
   {  3.671043614e-02,  1.029223793e-01 },
   { -1.566622157e-02, -2.717368690e-02 },
   { -3.052561472e-04,  3.107802594e-02 },
   { -5.275081924e-03, -1.727138294e-02 },
   {  3.390576685e-03,  3.029278004e-02 },
   {  2.050359608e-02,  2.887604020e-02 },
   { -2.039739883e-02,  2.958309353e-02 },
   { -2.641888998e-02,  4.148286192e-03 },
   { -8.344863252e-03, -1.067969983e-02 },
   { -6.583234075e-03,  4.237300217e-03 },
   {  6.881090346e-03, -1.692506287e-02 },
   {  5.214208808e-03,  5.081489587e-03 },
   {  2.073842435e-03,  1.153280420e-02 },
   { -4.004288906e-03, -2.358829492e-03 },
   {  5.331482002e-03,  2.076200879e-03 },
   {  3.579531627e-03,  1.921083064e-02 },
   {  9.681296791e-04, -1.189001985e-03 },
   { -1.438971625e-02,  2.720189933e-03 },
   { -8.666458347e-03,  1.030697371e-02 },
   { -2.226266050e-03,  1.612813350e-03 },
   { -6.588817930e-03, -2.276113762e-03 },
   {  9.661679548e-03,  1.667504264e-02 },
   { -4.293167128e-03, -4.712118702e-03 }
  };

  /* build forward and inverse fourier transforms. */
  hx::fft::forward<X::base_type, X::shape<0>> fwd;
  hx::fft::inverse<X::base_type, X::shape<0>> inv;

  /* initialize the schedule and input data. */
  Y y{obs};
  S sched{ids};
  X b, x, dx, fx;

  /* build the extended data vector. */
  b = y % sched;

  /* compute the initial thresholding value. */
  dx = b;
  fwd(dx.raw_data());
  double thresh = mu * dx.max().norm();

  // iterate.
  for (std::size_t it = 1; it <= iters; it++) {
    /* compute the current spectral estimate. */
    dx = b - x;
    dx *= sched;
    fwd(dx.raw_data());
    fx = fx + dx;

    /* apply the l1 function. */
    auto f = [thresh] (auto& z) {
      const double znrm = z.norm();
      z *= (znrm > thresh ? 1 - thresh / znrm : 0);
    };
    fx.foreach(f);

    /* update the time-domain estimate. */
    x = fx / n;
    inv(x.raw_data());

    /* update the threshold. */
    thresh *= mu;
  }

  /* output the result. */
  fwd(x.raw_data());
  x.foreach([] (auto& z) { std::cout << z << "\n"; });
}

