project(wide-decimal)

cmake_minimum_required(VERSION 3.16.3)

find_package(Boost)
if (Boost_FOUND)
  include(CTest)

  add_subdirectory("examples")
  add_subdirectory("test")
endif()

add_library(WideDecimal INTERFACE)
target_compile_features(WideDecimal INTERFACE cxx_std_11)

target_include_directories(
  WideDecimal SYSTEM INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>)

install(TARGETS WideDecimal EXPORT WideDecimalTargets)
install(
  FILES math/wide_decimal/decwide_t.h math/wide_decimal/decwide_t_detail.h math/wide_decimal/decwide_t_fft.h math/wide_decimal/decwide_t_namespace.h math/wide_decimal/decwide_t_default_ops.h
  DESTINATION include/math/wide_decimal/)
install(EXPORT WideDecimalTargets
  FILE WideDecimalConfig.cmake
  NAMESPACE WideDecimal::
  DESTINATION lib/cmake/wide-integer)
