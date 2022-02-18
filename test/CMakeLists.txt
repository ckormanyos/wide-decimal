enable_testing()
find_package(Threads)
if (Boost_FOUND)
  add_executable(test_decwide_t
  test_decwide_t_algebra.cpp
  test_decwide_t_examples.cpp
  test.cpp)
  target_compile_features(test_uintwide_t PRIVATE cxx_std_11)
  target_include_directories(test_uintwide_t PRIVATE ${PROJECT_SOURCE_DIR})
  target_link_libraries(test_uintwide_t Examples ${CMAKE_THREAD_LIBS_INIT})
  add_test(test test_uintwide_t)
endif()
