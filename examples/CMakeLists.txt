add_library(Examples
  example000a_multiply_pi_squared.cpp
  example000_multiply_nines.cpp
  example001a_roots_seventh.cpp
  example001b_roots_almost_integer.cpp
  example001c_roots_sqrt_limb08.cpp
  example001d_pow2_from_list.cpp
  example001_roots_sqrt.cpp
  example002a_pi_small_limb.cpp
  example002b_pi_100k.cpp
  example002c_pi_quintic.cpp
  example002d_pi_limb08.cpp
  example002_pi.cpp
  example003_zeta.cpp
  example004_bessel_recur.cpp
  example005_polylog_series.cpp
  example006_logarithm.cpp
  example007_catalan_series.cpp
  example008_bernoulli_tgamma.cpp
  example009a_boost_math_standalone.cpp
  example009b_boost_math_standalone.cpp
  example009_boost_math_standalone.cpp
  example010a_hypergeometric_1f1.cpp
  example010_hypergeometric_2f1.cpp
  example011_trig_trapezoid_integral.cpp
  example012_rational_floor_ceil.cpp
  example013a_embeddable_agm.cpp
  example013_embeddable_sqrt.cpp)
target_compile_features(Examples PRIVATE cxx_std_11)
target_include_directories(Examples PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(Examples SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
