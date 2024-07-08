# SPDX-License-Identifier: GPL-v2
#
# Copyright (c) 2023 EPAM Systems
# Author: Dmytro Semenets <dmytro_semenets@epam.com>

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(zephyr-optee-test)
find_package(Python REQUIRED)

set(EMBED_8100FILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/zephyr/include/generated)
macro(EMBED_8100FILE prefix infile)
	add_custom_command(
		OUTPUT  ${EMBED_8100FILE_DIR}/regression_8100_${prefix}.h
		COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/file_to_c.py --inf ${infile}
			--out ${EMBED_8100FILE_DIR}/regression_8100_${prefix}.h
			--name regression_8100_${prefix}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/file_to_c.py ${infile}
	)

	set_property(SOURCE src/regression_8100.c APPEND PROPERTY OBJECT_DEPENDS
		     ${EMBED_8100FILE_DIR}/regression_8100_${prefix}.h)
endmacro(EMBED_8100FILE)

EMBED_8100FILE(ca_crt ${CMAKE_CURRENT_SOURCE_DIR}/cert/ca.crt)
EMBED_8100FILE(mid_crt ${CMAKE_CURRENT_SOURCE_DIR}/cert/mid.crt)
EMBED_8100FILE(my_crt ${CMAKE_CURRENT_SOURCE_DIR}/cert/my.crt)
EMBED_8100FILE(my_csr ${CMAKE_CURRENT_SOURCE_DIR}/cert/my.csr)

zephyr_library_sources(adbg/src/adbg_expect.c)
zephyr_library_sources(adbg/src/adbg_log.c)
zephyr_library_sources(adbg/src/security_utils_hex.c)
zephyr_include_directories(adbg/include)

zephyr_library_include_directories(include)
zephyr_library_sources(src/optee_test.c)
zephyr_library_sources(src/xtest_helpers.c)

zephyr_library_sources(src/regression_1000.c)
zephyr_library_sources(src/pkcs11_1000.c)
zephyr_library_sources(src/regression_4000.c)
zephyr_library_sources(src/regression_4100.c)
zephyr_library_sources(src/regression_6000.c)
zephyr_library_sources(src/regression_8000.c)
zephyr_library_sources(src/regression_8100.c)
# ######################################################################################################################
# External libs
# ######################################################################################################################

zephyr_library_include_directories(${ZEPHYR_OPTEELIB_MODULE_DIR}/public)

# ######################################################################################################################
# Prebuilt TAs
# ######################################################################################################################

if(TA_DEPLOY_DIR)
    message(WARNING "Setting up TAs")
    set(TA_BIN_DIRECTORY ta_bin)
    set(GENERATED_DIRECTORY ta_table)
    add_custom_command(
        OUTPUT
            ${GENERATED_DIRECTORY}/ta_table.c
            ${GENERATED_DIRECTORY}/ta_table.S
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/prepare_ta.sh
                -s ${CMAKE_CURRENT_SOURCE_DIR}
                -f ${TA_DEPLOY_DIR}
                -b ${TA_BIN_DIRECTORY}
                -g ${GENERATED_DIRECTORY}
    )
    include_directories(app PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/${TA_BIN_DIRECTORY})
    zephyr_library_sources(${GENERATED_DIRECTORY}/ta_table.c)
    zephyr_library_sources(${GENERATED_DIRECTORY}/ta_table.S)

endif()
