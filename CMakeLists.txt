cmake_minimum_required(VERSION 3.20)
project(xc VERSION 1.0.7 LANGUAGES C CXX)

include(CMakePackageConfigHelpers)

option(LIBXC_BUILD_STATIC "Build static library" ON)
option(LIBXC_BUILD_TESTS "Build xc tests" OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

if(WIN32)
	set(CMAKE_STATIC_LIBRARY_PREFIX "")
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
	set(CMAKE_IMPORT_LIBRARY_PREFIX "")
	find_program(WINE_EXECUTABLE NAMES wine wine64)
endif()

# Build shared library
add_library(libxc SHARED ${CMAKE_SOURCE_DIR}/src/libxc.cc)
if(WIN32)
	target_link_libraries(libxc PRIVATE bcrypt)
	target_link_options(libxc PRIVATE -static -static-libgcc -static-libstdc++)
	target_link_options(libxc PRIVATE "LINKER:--output-def=${CMAKE_CURRENT_BINARY_DIR}/libxc.def")
endif()

# Set library version (for Unix)
if(NOT WIN32 AND NOT CMAKE_SYSTEM_NAME STREQUAL Windows)
	set_target_properties(libxc PROPERTIES
		VERSION ${PROJECT_VERSION}
		SOVERSION ${PROJECT_VERSION_MAJOR}
	)
endif()

# Include directories
target_include_directories(libxc PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(libxc PUBLIC pthread ${CMAKE_DL_LIBS})

if (LIBXC_BUILD_STATIC)
	add_library(libxc-static STATIC ${CMAKE_SOURCE_DIR}/src/libxc.cc)
	target_link_options(libxc-static PRIVATE -static -static-libgcc -static-libstdc++)
	set_target_properties(libxc-static PROPERTIES OUTPUT_NAME "libxc")
	target_include_directories(libxc-static PRIVATE ${CMAKE_SOURCE_DIR}/src)
	target_link_libraries(libxc-static PUBLIC pthread ${CMAKE_DL_LIBS})
	if(WIN32)
		target_link_libraries(libxc PRIVATE bcrypt)
		target_link_options(libxc-static PRIVATE -static -static-libgcc -static-libstdc++)
		target_link_options(libxc PRIVATE "LINKER:--output-def=${CMAKE_CURRENT_BINARY_DIR}/libxc.a.def")
	endif()
	set_target_properties(libxc-static PROPERTIES EXPORT_NAME libxc-static)
endif()

# -------------------
# Install Targets
# -------------------

# Install library
if (LIBXC_BUILD_STATIC)
	install(TARGETS libxc libxc-static
		EXPORT libxcTargets
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
	)
else()
	install(TARGETS libxc
		EXPORT libxcTargets
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
	)
endif()

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/libxc
	DESTINATION include
	FILES_MATCHING PATTERN "*.h"
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/doc/man3/
	DESTINATION share/man/man3
	FILES_MATCHING PATTERN "*.3"
)

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/libxcConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	"${CMAKE_SOURCE_DIR}/cmake/Config.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/libxcConfig.cmake"
	INSTALL_DESTINATION lib/cmake/libxc
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/libxcConfig.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/libxcConfigVersion.cmake"
	DESTINATION lib/cmake/libxc
)


if (LIBXC_BUILD_STATIC)
	export(TARGETS libxc libxc-static FILE libxcTargets.cmake)
else()
	export(TARGETS libxc FILE libxcTargets.cmake)
endif()

# Optional: install CMake config for find_package()
install(EXPORT libxcTargets
	FILE libxcTargets.cmake
	NAMESPACE libxc::
	DESTINATION lib/cmake/libxc
)

if(EXISTS "${CMAKE_SOURCE_DIR}/cmake/uninstall.cmake")
	add_custom_target(uninstall
		COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/uninstall.cmake
		COMMENT "Uninstalling libxc"
	)
endif()

# -------------------
# Tests
# -------------------
if (LIBXC_BUILD_TESTS)
	enable_testing()

	function(add_test_executable _NAME )
		add_executable(${_NAME} ${ARGN})
		target_include_directories(${_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/libxc)
		target_link_libraries(${_NAME} PRIVATE libxc)
		# Set output directories
		#set_target_properties(${_NAME} PROPERTIES
		#	ARCHIVE_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
		#	LIBRARY_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
		#	RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
		#)
		if (WINE_EXECUTABLE)
			add_test(NAME ${_NAME} COMMAND ${WINE_EXECUTABLE} $<TARGET_FILE:${_NAME}>)
		else()
			add_test(NAME ${_NAME} COMMAND $<TARGET_FILE:${_NAME}>)
		endif()
	endfunction()

	# Add executables for each test
	add_test_executable(test_xlocale ${CMAKE_SOURCE_DIR}/test/test_xlocale.c)
	add_test_executable(test_xstdio  ${CMAKE_SOURCE_DIR}/test/test_xstdio.c)
	add_test_executable(test_xstdlib ${CMAKE_SOURCE_DIR}/test/test_xstdlib.c)
	add_test_executable(test_xstring ${CMAKE_SOURCE_DIR}/test/test_xstring.c)
	add_test_executable(test_xwchar  ${CMAKE_SOURCE_DIR}/test/test_xwchar.c)
	add_test_executable(test_xctype  ${CMAKE_SOURCE_DIR}/test/test_xctype.c)
	add_test_executable(test_xwctype ${CMAKE_SOURCE_DIR}/test/test_xwctype.c)
	add_test_executable(test_xstat   ${CMAKE_SOURCE_DIR}/test/test_xstat.c)

	add_test_executable(test_xdlfcn  ${CMAKE_SOURCE_DIR}/test/test_xdlfcn.c)
	if(NOT WIN32)
		target_link_options(test_xdlfcn PRIVATE -rdynamic)
	endif()

	add_test_executable(test_xmap    ${CMAKE_SOURCE_DIR}/test/test_xmap.cc)
	if(WIN32)
		set_target_properties(test_xmap PROPERTIES LINKER_LANGUAGE CXX)
		target_link_libraries(test_xmap PRIVATE stdc++)
		target_link_options(test_xmap PRIVATE -static -static-libgcc -static-libstdc++)
	endif()
endif()
