# Copyright (c) 2026 LVGL
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

# Per-board defaults: boards/<board>.cmake sets variables (e.g. SHIELD) not already provided.
# Normalize the raw BOARD spec by stripping @<revision> and replacing / with _ to match the
# naming convention used for boards/<board>.conf and boards/<board>.overlay.
string(REGEX REPLACE "@[^/]*" "" _board_norm "${BOARD}")
string(REPLACE "/" "_" _board_norm "${_board_norm}")
include("${CMAKE_CURRENT_SOURCE_DIR}/boards/${_board_norm}.cmake" OPTIONAL)
unset(_board_norm)

# Board overlay loading order:
#   1. boards/pre-shield-overlays/<board>.overlay — via BOARD_EXTENSION_DIRS, applied before shield overlays
#   2. shield overlay                              — applied by west (--shield flag)
#   3. boards/<board>.overlay                      — auto-discovered by Zephyr, applied after shield overlays
list(APPEND BOARD_EXTENSION_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/boards/pre-shield-overlays)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(lvgl_application)

# The Editor-generated UI includes LVGL as `#include "lvgl.h"`.
zephyr_compile_definitions(LV_LVGL_H_INCLUDE_SIMPLE)

# Application sources.
target_sources(app PRIVATE src/main.c)

# Build the UI exported from the LVGL Editor. ui/CMakeLists.txt produces a
# `lib-ui` library; link it to the app and give it LVGL's include paths.
add_subdirectory(ui)
target_link_libraries(lib-ui PUBLIC zephyr_interface)
target_link_libraries(app PRIVATE lib-ui)
