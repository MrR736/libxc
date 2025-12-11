cmake_minimum_required(VERSION 3.20)
project(xc VERSION 1.0.3 LANGUAGES C CXX)

include(CMakePackageConfigHelpers)

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
if(WIN32)
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


add_library(${PROJECT_NAME}-static STATIC ${CMAKE_SOURCE_DIR}/src/libxc.cc)
target_link_options(${PROJECT_NAME}-static PRIVATE -static -static-libgcc -static-libstdc++)
set_target_properties(${PROJECT_NAME}-static PROPERTIES
    OUTPUT_NAME "xc"
    ARCHIVE_OUTPUT_DIRECTORY ${LIBS_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${LIBS_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${LIBS_OUTPUT_DIR}
)
target_include_directories(${PROJECT_NAME}-static PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME}-static PUBLIC pthread)

set_target_properties(${PROJECT_NAME}-static PROPERTIES
    EXPORT_NAME ${PROJECT_NAME}-static
)

if(NOT WIN32)
    # -------------------
    # Install Targets
    # -------------------

    # Install library
    install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}-static
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

    export(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}-static FILE ${PROJECT_NAME}Targets.cmake)

    # Optional: install CMake config for find_package()
    install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION lib/cmake/${PROJECT_NAME}
    )

    if(EXISTS "${CMAKE_SOURCE_DIR}/cmake/uninstall.cmake")
        add_custom_target(uninstall
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/uninstall.cmake
            COMMENT "Uninstalling ${PROJECT_NAME}"
        )
    endif()
endif()

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
    ${CMAKE_SOURCE_DIR}/test/test_xctype.c
    ${CMAKE_SOURCE_DIR}/test/test_xwctype.c
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

if(WIN32)
    set_target_properties(test_xmap_cc PROPERTIES LINKER_LANGUAGE CXX)
    target_link_libraries(test_xmap_cc PRIVATE stdc++)
endif()
