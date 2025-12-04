cmake_minimum_required(VERSION 3.20)
project(xc VERSION 1.0.0 LANGUAGES C CXX)

include(CMakePackageConfigHelpers)

option(BUILD_WINDOWS "Use Windows" OFF)

if(BUILD_WINDOWS OR WIN32)
include(Platform/Windows)
include(Platform/Windows-windres)

if(CMAKE_C_COMPILER_LINKER_ID STREQUAL "GNU"
   OR CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
	include(Platform/Windows-GNU-C)
	include(Platform/Linker/Windows-GNU-C)
else()
	include(Platform/Windows-MSVC-C)
	include(Platform/Linker/Windows-MSVC-C)
endif()

if(CMAKE_CXX_COMPILER_LINKER_ID STREQUAL "GNU"
   OR CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
	include(Platform/Windows-GNU-CXX)
	include(Platform/Linker/Windows-GNU-CXX)
else()
	include(Platform/Windows-MSVC-CXX)
	include(Platform/Linker/Windows-MSVC-CXX)
endif()

# --- Setup cross compilation ---
set(CMAKE_SYSTEM_NAME Windows)

if(NOT DEFINED CMAKE_SIZEOF_VOID_P)
    message(WARNING "CMAKE_SIZEOF_VOID_P not defined; assuming 64-bit build.")
    set(CMAKE_SIZEOF_VOID_P 8)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(CROSS_PREFIX i686-w64-mingw32)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CROSS_PREFIX x86_64-w64-mingw32)
endif()

set(CMAKE_RC_COMPILER  ${CROSS_PREFIX}-windres)
set(CMAKE_C_COMPILER   ${CROSS_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${CROSS_PREFIX}-g++)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_C_COMPILER "${CROSS_PREFIX}-gcc")
set(CMAKE_CXX_COMPILER "${CROSS_PREFIX}-g++")
set(CMAKE_CXX_COMPILER_AR "${CROSS_PREFIX}-gcc-ar")
set(CMAKE_CXX_COMPILER_RANLIB "${CROSS_PREFIX}-gcc-ranlib")
# set(CMAKE_CXX_COMPILER_LINKER "${CROSS_PREFIX}-ld")
set(CMAKE_AR "${CROSS_PREFIX}-ar")
set(CMAKE_RANLIB "${CROSS_PREFIX}-ranlib")
set(CMAKE_OBJDUMP "${CROSS_PREFIX}-objdump")
set(CMAKE_OBJCOPY "${CROSS_PREFIX}-objcopy")
set(CMAKE_READELF "${CROSS_PREFIX}-readelf")
set(CMAKE_NM "${CROSS_PREFIX}-nm")
# set(CMAKE_LINKER "${CROSS_PREFIX}-ld")
set(CMAKE_STRIP "${CROSS_PREFIX}-strip")
set(CMAKE_ADDR2LINE "${CROSS_PREFIX}-addr2line")
set(CMAKE_DLLTOOL "${CROSS_PREFIX}-dlltool")
set(CMAKE_DLLWRAP "${CROSS_PREFIX}-dllwrap")
set(CMAKE_RC_COMPILER "${CROSS_PREFIX}-windres")

set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_SHARED_LINKER_FLAGS "")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Output directories
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}")
set(LIBS_OUTPUT_DIR "${OUTPUT_DIR}/lib")
set(TEST_OUTPUT_DIR "${OUTPUT_DIR}/test")
file(MAKE_DIRECTORY "${LIBS_OUTPUT_DIR}" "${TEST_OUTPUT_DIR}")

# Build shared library
add_library(${PROJECT_NAME} SHARED ${CMAKE_SOURCE_DIR}/src/libxc.cc)
if(BUILD_WINDOWS OR WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE -static -static-libgcc -static-libstdc++)
endif()

# Set library version (for Unix)
if(NOT WIN32 AND NOT CMAKE_SYSTEM_NAME STREQUAL Windows)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
    )
endif()

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${LIBS_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${LIBS_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${LIBS_OUTPUT_DIR}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)

set_target_properties(${PROJECT_NAME} PROPERTIES
    EXPORT_NAME ${PROJECT_NAME}
)

# -------------------
# Install Targets
# -------------------

# Install library
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/doc/
    DESTINATION share/man/man3
    FILES_MATCHING PATTERN "*.3"
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/doc/
    DESTINATION share/doc/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.md"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME}
)

export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}Targets.cmake)

# Optional: install CMake config for find_package()
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# -------------------
# Tests
# -------------------
enable_testing()

# List of test files
set(TEST_FILES
    ${CMAKE_SOURCE_DIR}/test/test_xlocale.c
    ${CMAKE_SOURCE_DIR}/test/test_xmap.cc
    ${CMAKE_SOURCE_DIR}/test/test_xstdio.c
    ${CMAKE_SOURCE_DIR}/test/test_xstdlib.c
    ${CMAKE_SOURCE_DIR}/test/test_xstring.c
    ${CMAKE_SOURCE_DIR}/test/test_xwchar.c
)

# Add executables for each test
foreach(test_src ${TEST_FILES})
    get_filename_component(test_name ${test_src} NAME_WE)
    get_filename_component(ext ${test_src} EXT)
    # Replace '.' in extension with '_' and append
    string(REPLACE "." "_" ext_suffix ${ext})
    set(target_name "${test_name}${ext_suffix}")

    add_executable(${target_name} ${test_src})
    target_include_directories(${target_name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${target_name} PRIVATE ${PROJECT_NAME} pthread)
    # Set output directories
    set_target_properties(${target_name} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
    )
    add_test(NAME ${target_name} COMMAND ${target_name})
endforeach()

if(BUILD_WINDOWS OR WIN32)
    set_target_properties(test_xmap_cc PROPERTIES LINKER_LANGUAGE CXX)
    target_link_libraries(test_xmap_cc PRIVATE stdc++)
endif()
