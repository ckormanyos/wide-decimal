﻿///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2022.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_cpu.h>
#include <mcal_osc.h>
#include <mcal_reg.h>

void mcal::cpu::init()
{
  // Initialize the fpu: Enable cp10 and cp11.
  // Note: The address of scb_cpacr is 0xE000ED88.
  mcal::reg::reg_access_static<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::scb_cpacr,
                               UINT32_C(0x00F00000)>::reg_or();

  // Reset the rcc clock configuration to the default reset state
  // Set the hsion bit.
  mcal::reg::reg_access_static<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::rcc_cr,
                               UINT32_C(0x00000001)>::reg_or();

  // Reset the rcc_cfgr register.
  mcal::reg::reg_access_static<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::rcc_cfgr,
                               UINT32_C(0)>::reg_set();

  // Clear the hseon, the csson, and the pllon bits.
  mcal::reg::reg_access_static<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::rcc_cr,
                               UINT32_C(0xFEF6FFFF)>::reg_and();

  // Reset the pllcfgr register.
  mcal::reg::reg_access_static<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::rcc_pllcfgr,
                               UINT32_C(0x24003010)>::reg_set();

  // Reset the hsebyp bit.
  mcal::reg::reg_access_static<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::rcc_cr,
                               UINT32_C(0xFFFBFFFF)>::reg_and();

  // Disable all interrupts.
  mcal::reg::reg_access_static<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::rcc_cir,
                               UINT32_C(0)>::reg_set();

  // Configure the flash memory.
  // Set the wait states.
  // Activate the flash prefetch.
  // Activate the instruction cache.
  // Activate the data cache.
  constexpr std::uint32_t wait_states = UINT32_C(5);

  mcal::reg::reg_access_static<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::flash_acr,
                               wait_states | UINT32_C(0x00000700)>::reg_set();

  // Enable the power interface clock.
  mcal::reg::reg_access_static<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::rcc_apb1enr,
                               UINT32_C(0x10000000)>::reg_or();

  // Select the regulator voltage scaling output to "scale 3 mode".
  mcal::reg::reg_access_static<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::pwr_cr,
                               UINT32_C(0x0000C000)>::reg_or();

  mcal::osc::init(nullptr);
}
