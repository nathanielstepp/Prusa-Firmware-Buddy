target_include_directories(firmware PRIVATE .)

add_subdirectory(device)
add_subdirectory(segger)
add_subdirectory(hw)
add_subdirectory(logging)
add_subdirectory(common)

if(BOARD MATCHES "BUDDY")
  if(HAS_ESP_FLASH_TASK)
    add_subdirectory(buddy-esp-serial-flasher)
    target_link_libraries(firmware PRIVATE buddy-esp-serial-flasher)
  endif()

  add_subdirectory(persistent_stores)

  if(CONNECT OR WUI)
    add_subdirectory(transfers)
    add_subdirectory(state)
  endif()

  if(CONNECT)
    if(NOT WUI)
      message(FATAL_ERROR "Connect needs WUI!")
    endif()
    add_subdirectory(connect)
  endif()

  if(RESOURCES)
    add_subdirectory(resources)
  endif()

  if(BOOTLOADER)
    add_subdirectory(bootloader)
  endif()

  if(GUI)
    add_subdirectory(gui)
    add_subdirectory(guiapi)
  endif()

  if(HAS_PUPPIES)
    add_subdirectory(puppies)
  endif()

  if(HAS_LEDS)
    add_subdirectory(leds)
  endif()

  add_subdirectory(buddy)
  add_subdirectory(lang)
  add_subdirectory(marlin_stubs)
  add_subdirectory(syslog)
  if(HAS_MMU2)
    add_subdirectory(mmu2)
  endif()

  add_subdirectory(semihosting)
elseif(BOARD STREQUAL "DWARF")
  add_subdirectory(puppy/dwarf)
  add_subdirectory(puppy/shared)
elseif(BOARD STREQUAL "MODULARBED")
  add_subdirectory(puppy/modularbed)
  add_subdirectory(puppy/shared)
elseif(BOARD STREQUAL "XL_DEV_KIT_XLB")
  add_subdirectory(persistent_stores)
  add_subdirectory(xl_dev_kit)
  add_subdirectory(buddy)
  if(HAS_PUPPIES)
    add_subdirectory(puppies)
  endif()
  add_subdirectory(marlin_stubs)
endif()

target_sources(firmware PRIVATE freertos.cpp)
