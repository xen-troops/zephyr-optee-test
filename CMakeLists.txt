# SPDX-License-Identifier: GPL-v2
#
# Copyright (c) 2023 EPAM Systems
# Author: Dmytro Semenets <dmytro_semenets@epam.com>

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(zephyr-optee-test)

zephyr_library_sources(adbg/src/adbg_expect.c)
zephyr_library_sources(adbg/src/adbg_log.c)
zephyr_library_sources(adbg/src/security_utils_hex.c)
zephyr_include_directories(adbg/include)

zephyr_library_include_directories(include)
zephyr_library_sources(src/optee_test.c)
zephyr_library_sources(src/xtest_helpers.c)

zephyr_library_sources(src/regression_1000.c)
zephyr_library_sources(src/pkcs11_1000.c)
# ######################################################################################################################
# External libs
# ######################################################################################################################

zephyr_library_include_directories(${ZEPHYR_OPTEELIB_MODULE_DIR}/public)
