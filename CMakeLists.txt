project(wide-decimal)

cmake_minimum_required(VERSION 3.16.3)

find_package(Boost)
if (Boost_FOUND)
  include(CTest)

  add_subdirectory("examples")
  add_subdirectory("test")
endif()

add_library(WideDecimal INTERFACE)
target_compile_features(WideDecimal INTERFACE cxx_std_20)

target_include_directories(
  WideDecimal SYSTEM INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>)

install(TARGETS WideDecimal EXPORT WideDecimalTargets)

install(
  FILES math/wide_decimal/decwide_t.h math/wide_decimal/decwide_t_detail.h
    math/wide_decimal/decwide_t_detail_fft.h math/wide_decimal/decwide_t_detail_namespace.h
    math/wide_decimal/decwide_t_detail_ops.h
  DESTINATION include/math/wide_decimal/)

install(
  FILES math/constants/constants_pi_control_for_decwide_t.h
DESTINATION include/math/constants/)

install(
  FILES mcal_lcd/mcal_lcd_console.h mcal_lcd/mcal_lcd_base.h mcal_lcd/mcal_lcd_generic_st7066.h
DESTINATION include/mcal_lcd/)

install(
  FILES util/utility/util_baselexical_cast.h util/utility/util_dynamic_array.h
    util/utility/util_noncopyable.h
  DESTINATION include/util/utility/
)

install(
  FILES util/memory/util_n_slot_array_allocator.h
  DESTINATION include/util/memory/
)

install(
  FILES util/stdcpp/stdcpp_patch.cpp
  DESTINATION include/util/stdcpp/
)

install(
  FILES boost/math/bindings/decwide_t.hpp
  DESTINATION include/boost/math/bindings/
)

install(EXPORT WideDecimalTargets
  FILE WideDecimalConfig.cmake
  NAMESPACE WideDecimal::
  DESTINATION lib/cmake/wide-decimal)
