###############################################################################
#  Copyright Christopher Kormanyos 2026.
#  Distributed under the Boost Software License,
#  Version 1.0. (See accompanying file LICENSE_1_0.txt
#  or copy at http://www.boost.org/LICENSE_1_0.txt)
#

# My local machine
# cd /mnt/c/Users/ckorm/Documents/Ks/PC_Software/NumericalPrograms/ExtendedNumberTypes/wide_decimal
# make MY_CC=g++ MY_BOOST_ROOT=/mnt/c/boost/boost_1_90_0 all

# MacOS with brew llvm
# make MY_CC=/opt/homebrew/opt/llvm/bin/clang++ MY_BOOST_ROOT=../boost-root MY_STD=c++23 MY_BREW=brew all

all : compile_and_link


BOOST_ROOT   := ../boost-root
CC           := clang++
STD          := c++20


ifneq ($(MY_BOOST_ROOT),)
BOOST_ROOT   := $(MY_BOOST_ROOT)
endif

ifneq ($(MY_CC),)
CC           := $(MY_CC)
endif

ifneq ($(MY_STD),)
STD          := $(MY_STD)
endif

ECHO         := echo


FILES_SRC    := test/test.cpp                                            \
                test/test_decwide_t_algebra.cpp                          \
                test/test_decwide_t_algebra_edge.cpp                     \
                test/test_decwide_t_examples.cpp                         \
                examples/example000a_multiply_pi_squared.cpp             \
                examples/example000_multiply_nines.cpp                   \
                examples/example001_roots_sqrt.cpp                       \
                examples/example001a_roots_seventh.cpp                   \
                examples/example001b_roots_almost_integer.cpp            \
                examples/example001c_roots_sqrt_limb08.cpp               \
                examples/example001d_pow2_from_list.cpp                  \
                examples/example001e_algebra_and_constexpr.cpp           \
                examples/example002_pi.cpp                               \
                examples/example002a_pi_small_limb.cpp                   \
                examples/example002b_pi_100k.cpp                         \
                examples/example002c_pi_quintic.cpp                      \
                examples/example002d_pi_limb08.cpp                       \
                examples/example003_zeta.cpp                             \
                examples/example004_bessel_recur.cpp                     \
                examples/example005_polylog_series.cpp                   \
                examples/example006_logarithm.cpp                        \
                examples/example007_catalan_series.cpp                   \
                examples/example008_bernoulli_tgamma.cpp                 \
                examples/example009_boost_math_standalone.cpp            \
                examples/example009a_boost_math_standalone.cpp           \
                examples/example009b_boost_math_standalone.cpp           \
                examples/example010_hypergeometric_2f1.cpp               \
                examples/example010a_hypergeometric_1f1.cpp              \
                examples/example011_trig_trapezoid_integral.cpp          \
                examples/example012_rational_floor_ceil.cpp              \
                examples/example013_embeddable_sqrt.cpp                  \
                examples/example013a_embeddable_agm.cpp


CPPFLAGS     := -std=$(STD)                                              \
                -finline-functions                                       \
                -march=native                                            \
                -mtune=native                                            \
                -O3                                                      \
                -Werror                                                  \
                -Wall                                                    \
                -Wextra                                                  \
                -Wpedantic                                               \
                -Wconversion                                             \
                -Wsign-conversion                                        \
                -Wshadow                                                 \
                -Wundef                                                  \
                -Wunused-parameter                                       \
                -Wuninitialized                                          \
                -Wunreachable-code                                       \
                -Winit-self                                              \
                -Wzero-as-null-pointer-constant


C_DEFINES    := -DWIDE_DECIMAL_NAMESPACE=ckormanyos

C_INCLUDES   := -I.                                                      \
                -I$(BOOST_ROOT)


ifeq ($(MY_BREW),brew)
C_INCLUDES   += -isystem /opt/homebrew/opt/llvm/include
C_INCLUDES   += -isystem $$(xcrun --show-sdk-path)
endif


LDFLAGS      := -pthread                                                 \
                -lpthread


ifeq ($(MY_BREW),brew)
LDFLAGS      += -L/opt/homebrew/opt/llvm/lib
endif

clean :
	@-$(ECHO)
	@-$(ECHO) +++ cleaning
	@-rm -f ./wide_integer || true


compile_and_link : clean
	@-$(ECHO)
	@-$(ECHO) +++ print compiler version
	@-$(CC) --version
	@-$(ECHO)
	@-$(ECHO) +++ compiling and linking to wide_decimal
	@-$(ECHO)
	-$(CC) $(CPPFLAGS) $(C_DEFINES) $(C_INCLUDES) $(LDFLAGS) $(FILES_SRC) -o wide_decimal
	@-$(ECHO)
	@-$(ECHO) +++ check for executable wide_decimal
	-ls -la ./wide_decimal
	@-$(ECHO)
