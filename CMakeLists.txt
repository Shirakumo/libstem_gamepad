cmake_minimum_required(VERSION 3.1)
project(stem_gamepad)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_BUILD_TYPE Release)

add_library(stem_gamepad SHARED)

include_directories(source)
target_sources(stem_gamepad PUBLIC "source/gamepad/Gamepad_private.c")
set_property(TARGET stem_gamepad PROPERTY C_STANDARD 99)
set_property(TARGET stem_gamepad PROPERTY POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(stem_gamepad PRIVATE STEM_GAMEPAD_BUILD=1)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang"
    OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(stem_gamepad PRIVATE -fvisibility=hidden)
endif()

if(UNIX AND NOT APPLE)
  target_sources(stem_gamepad PUBLIC "source/gamepad/Gamepad_linux.c")
endif()

if(APPLE)
  find_library(CORE_FOUNDATION CoreFoundation)
  find_library(IOKit_LIBRARY IOKit)
  target_link_libraries(stem_gamepad ${IOKit_LIBRARY} ${CORE_FOUNDATION})
  target_sources(stem_gamepad PUBLIC "source/gamepad/Gamepad_macosx.c")
endif()

if(WIN32)
  find_package(WindowsSDK REQUIRED)
  include_directories(${WINSDK_INCLUDE_DIRS})
  target_compile_options(stem_gamepad PRIVATE -D_MSC_VER=1500)
  target_link_libraries(stem_gamepad ${WINSDK_LIBRARY_DIRS})
  target_sources(stem_gamepad PUBLIC "source/gamepad/Gamepad_windows_dinput.c")
endif()


if(WIN32)
endif()

install(TARGETS stem_gamepad DESTINATION /usr/lib)
