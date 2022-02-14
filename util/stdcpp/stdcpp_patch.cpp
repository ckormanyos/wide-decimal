#if defined(__GNUC__) && defined(__AVR__)
#include <cstddef>

void operator delete(void*)        noexcept;
void operator delete(void*, void*) noexcept;
#if(__cplusplus >= 201400L)
void operator delete(void*, std::size_t) noexcept;
#endif

void operator delete(void*)              noexcept { }
void operator delete(void*, void*)       noexcept { }
#if(__cplusplus >= 201400L)
void operator delete(void*, std::size_t) noexcept { }
#endif
#else
void stdcpp_patch_dummy() noexcept;

void stdcpp_patch_dummy() noexcept { }
#endif
