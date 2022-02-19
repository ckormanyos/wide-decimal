#ifndef UTIL_N_SLOT_ARRAY_ALLOCATOR_2020_10_25_H
  #define UTIL_N_SLOT_ARRAY_ALLOCATOR_2020_10_25_H

  #include <algorithm>
  #include <array>
  #include <cstddef>
  #include <cstdint>

  namespace util {

  // Forward declaration of n_slot_array_allocator template.
  template<typename T,
           const std::uint_fast32_t SlotWidth,
           const std::size_t SlotCount>
  class n_slot_array_allocator;

  // Template partial specialization of n_slot_array_allocator template for void.
  template<const std::uint_fast32_t SlotWidth,
           const std::size_t SlotCount>
  class n_slot_array_allocator<void, SlotWidth, SlotCount>
  {
  public:
    using value_type    = void;
    using pointer       = value_type*;
    using const_pointer = const value_type*;

    template<typename U>
    struct rebind
    {
      using other = n_slot_array_allocator<U, SlotWidth, SlotCount>;
    };
  };

  template<typename T,
           const std::uint_fast32_t SlotWidth,
           const std::size_t SlotCount>
  class n_slot_array_allocator // NOLINT(cppcoreguidelines-special-member-functions,hicpp-special-member-functions)
  {
  private:
    static constexpr std::uint_fast32_t slot_width = SlotWidth;
    static constexpr std::size_t        slot_count = SlotCount;

    using slot_array_type = std::array<T, slot_width>;

  public:
    using size_type       = std::size_t;
    using value_type      = typename slot_array_type::value_type;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    n_slot_array_allocator() = default;

    n_slot_array_allocator(const n_slot_array_allocator&) = default;

    template<typename U>
    struct rebind
    {
      using other = n_slot_array_allocator<U, SlotWidth, SlotCount>;
    };

    auto max_size() const -> size_type
    {
      return sizeof(slot_array_type) * slot_count;
    }

    auto address(      reference x) const ->       pointer { return &x; }
    auto address(const_reference x) const -> const_pointer { return &x; }

    auto allocate
    (
      size_type                                                                    count,
      typename n_slot_array_allocator<void, slot_width, slot_count>::const_pointer p_hint = nullptr
    ) -> pointer
    {
      static_cast<void>(count);
      static_cast<void>(p_hint);

      pointer p = nullptr;

      for(std::size_t i = 0U; i < slot_count; ++i)
      {
        if(slot_flags[i] == 0U)
        {
          slot_flags[i] = 1U;

          p = (pointer) slot_array_memory[i].data();

          if(i > slot_max_index)
          {
            slot_max_index = i;

            static_cast<void>(slot_max_index);
          }

          break;
        }
      }

      return p;
    }

    auto construct(pointer p, const value_type& x) -> void
    {
      // The memory in the n-slot allocator already exists
      // in an uninitialized form. Construction can safely
      // simply set the value in the uninitialized memory.

      *p = x;
    }

    auto destroy(pointer p) -> void
    {
      static_cast<void>(p);
    }

    auto deallocate(pointer p_slot, size_type) -> void
    {
      for(std::size_t i = 0U; i < slot_count; ++i)
      {
        if(p_slot == (pointer) slot_array_memory[i].data())
        {
          slot_flags[i] = 0U;

          break;
        }
      }
    }

  private:
    static std::array<slot_array_type, slot_count> slot_array_memory;
    static std::array<std::uint8_t, slot_count>    slot_flags;
    static std::size_t                             slot_max_index;
  };

  template<typename T,
           const std::uint_fast32_t SlotWidth,
           const std::size_t SlotCount>
  std::array<typename n_slot_array_allocator<T, SlotWidth, SlotCount>::slot_array_type, n_slot_array_allocator<T, SlotWidth, SlotCount>::slot_count>
  n_slot_array_allocator<T, SlotWidth, SlotCount>::slot_array_memory;

  template<typename T,
           const std::uint_fast32_t SlotWidth,
           const std::size_t SlotCount>
  std::array<std::uint8_t, n_slot_array_allocator<T, SlotWidth, SlotCount>::slot_count>
  n_slot_array_allocator<T, SlotWidth, SlotCount>::slot_flags;

  template<typename T,
           const std::uint_fast32_t SlotWidth,
           const std::size_t SlotCount>
  std::size_t n_slot_array_allocator<T, SlotWidth, SlotCount>::slot_max_index;

  // Global comparison operators (required by the standard).
  template<typename T,
           const std::uint_fast32_t SlotWidth,
           const std::size_t SlotCount>
  auto operator==(const n_slot_array_allocator<T, SlotWidth, SlotCount>&,
                  const n_slot_array_allocator<T, SlotWidth, SlotCount>&) -> bool
  {
    return true;
  }

  template<typename T,
           const std::uint_fast32_t SlotWidth,
           const std::size_t SlotCount>
  auto operator!=(const n_slot_array_allocator<T, SlotWidth, SlotCount>&,
                  const n_slot_array_allocator<T, SlotWidth, SlotCount>&) -> bool
  {
    return false;
  }

  } // namespace util

#endif // UTIL_N_SLOT_ARRAY_ALLOCATOR_2020_10_25_H
