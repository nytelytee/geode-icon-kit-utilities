cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(GEODE_SET_TARGET_AS_SYSTEM ON)

project(IconKitUtilities VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED
  src/hooks/GJGarageLayer.cpp
  src/hooks/ItemInfoPopup.cpp
  src/hooks/FLAlertLayer.cpp
  src/hooks/CCLayerColor.cpp
  src/hooks/CharacterColorPage.cpp
  
  src/utils/LinkedCCMenu.cpp
  src/utils/CCMenuItemTogglerSpoof.cpp

  src/popups/PlayerOptionsPopup.cpp

  src/main.cpp
)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_sources(${PROJECT_NAME} PRIVATE src/debug.cpp)
endif()
include_directories(${PROJECT_NAME} PUBLIC include)


if (NOT DEFINED ENV{GEODE_SDK})
  message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
  message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

if (MSVC)
  target_compile_options(
    ${PROJECT_NAME} PUBLIC
    "/Wall"
    "/wd4820"  # padding inserted
    "/wd4710"  # function not inlined
    "/wd5045"  # disable spectre mitigation warning
    # implicitly marked as deleted
    "/wd4625"
    "/wd5026"
    "/wd4626"
    "/wd5027"
  )
else()
  target_compile_options(
    ${PROJECT_NAME} PUBLIC
    "-Wall" "-Wextra" "-Wpedantic"
    "-Wno-dollar-in-identifier-extension"
    "-Wno-c99-designator"
  )
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)

# Set up dependencies, resources, link Geode
setup_geode_mod(${PROJECT_NAME})
