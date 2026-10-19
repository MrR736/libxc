cmake_minimum_required(VERSION 3.20)
project(xc
	VERSION 1.0.8
	DESCRIPTION "Extern Utility Library"
	LANGUAGES C CXX
)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

option(LIBXC_BUILD_SHARED "Build shared library" ON)
option(LIBXC_BUILD_STATIC "Build static library" ON)
option(LIBXC_INSTALL_MAN3 "Install man3" OFF)
option(LIBXC_BUILD_TESTS "Build xc tests" OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
	set(CMAKE_STATIC_LIBRARY_PREFIX "")
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
	set(CMAKE_IMPORT_LIBRARY_PREFIX "")
	find_program(WINE_EXECUTABLE NAMES wine wine64)
endif()

if(NOT LIBXC_BUILD_SHARED AND NOT LIBXC_BUILD_STATIC)
	message(FATAL_ERROR "At least one of LIBXC_BUILD_SHARED or LIBXC_BUILD_STATIC must be enabled.")
endif()

set(LIBXC_SOURCES ${CMAKE_SOURCE_DIR}/src/libxc.cc)

set(LIBXC_TARGETS)
set(LIBXC_LIBS)
set(ILIBXC_LDFLAGS)

if(NOT DEFINED LIBXC_INSTALL_PREFIX)
	set(LIBXC_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
endif()

if(WIN32)
	list(APPEND LIBXC_LIBS bcrypt)
else()
	if(UNIX)
		list(APPEND LIBXC_LIBS m)
	endif()
	list(APPEND LIBXC_LIBS pthread ${CMAKE_DL_LIBS})
endif()


foreach(libxc_dep IN LISTS LIBXC_LIBS)
	if(libxc_dep MATCHES "^[A-Za-z0-9_+-]+$")
		list(APPEND ILIBXC_LDFLAGS "-l${libxc_dep}")
	else()
		list(APPEND ILIBXC_LDFLAGS "${libxc_dep}")
	endif()
endforeach()

string(REPLACE ";" " " ILIBXC_LDFLAGS "${ILIBXC_LDFLAGS}")

# Build shared library
if (LIBXC_BUILD_SHARED)
	add_library(libxc SHARED ${LIBXC_SOURCES})
	target_compile_definitions(libxc PRIVATE XSTDAPI_BUILD)

	# Set library version
	if(NOT WIN32)
		set_target_properties(libxc PROPERTIES
			VERSION ${PROJECT_VERSION}
			SOVERSION ${PROJECT_VERSION_MAJOR}
		)
	endif()

	# Include directories
	target_include_directories(libxc
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/libxc>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/libxcxx>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	)

	target_link_libraries(libxc PUBLIC ${LIBXC_LIBS})
	if(WIN32)
		target_link_options(libxc PUBLIC -static -static-libgcc -static-libstdc++)
		target_link_options(libxc PRIVATE "LINKER:--output-def=${CMAKE_CURRENT_BINARY_DIR}/libxc.def")
	endif()


	set(LIBXC_ADD_TEXT "")
	set(LIBXC_LDFLAGS "-llibxc")
	set(LIBXC_LDFLAGS_PRIVATE "${ILIBXC_LDFLAGS}")
	set(LIBXC_CFLAGS "")
	set(LIBXC_DESCRIPTION "")
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/libxc.pc.in ${CMAKE_CURRENT_BINARY_DIR}/libxc.pc @ONLY)

	list(APPEND LIBXC_TARGETS libxc)
endif()

if (LIBXC_BUILD_STATIC)
	add_library(libxc-static STATIC ${LIBXC_SOURCES})
	target_compile_definitions(libxc-static PUBLIC XSTDAPI_STATIC)
	set_target_properties(libxc-static PROPERTIES OUTPUT_NAME "libxc")
	target_include_directories(libxc-static
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/libxc>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/libxcxx>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	)

	target_link_libraries(libxc-static PUBLIC ${LIBXC_LIBS})
	if(WIN32)
		target_link_options(libxc-static PUBLIC -static -static-libgcc -static-libstdc++)
	endif()

	set(LIBXC_ADD_TEXT " Static")
	set(LIBXC_LDFLAGS "-Wl,-Bstatic -llibxc -Wl,-Bdynamic")
	set(LIBXC_LDFLAGS_PRIVATE "${ILIBXC_LDFLAGS}")
	set(LIBXC_CFLAGS " -DXSTDAPI_STATIC")
	set(LIBXC_DESCRIPTION "(static)")
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/libxc.pc.in ${CMAKE_CURRENT_BINARY_DIR}/libxc-static.pc @ONLY)

	list(APPEND LIBXC_TARGETS libxc-static)
endif()

add_library(libxc-single INTERFACE)

target_include_directories(libxc-single INTERFACE
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/libxc-single>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/libxcxx>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(libxc-single INTERFACE ${LIBXC_LIBS})

set(LIBXC_LDFLAGS_PRIVATE "${ILIBXC_LDFLAGS}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/libxc-single.pc.in ${CMAKE_CURRENT_BINARY_DIR}/libxc-single.pc @ONLY)

# -------------------
# Install Targets
# -------------------

# Install library
install(TARGETS ${LIBXC_TARGETS} libxc-single
	EXPORT libxcTargets
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libxc-single
	FILES_MATCHING PATTERN "*.h"
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/libxc
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILES_MATCHING PATTERN "*.h"
)

install(
	DIRECTORY ${CMAKE_SOURCE_DIR}/include/libxcxx
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	PATTERN ".clangd" EXCLUDE
	PATTERN ".clang-tidy" EXCLUDE
)

if(LIBXC_INSTALL_MAN3)
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/doc/man3/
			DESTINATION ${CMAKE_INSTALL_MANDIR}/man3
			FILES_MATCHING PATTERN "*.3"
	)
endif()

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/libxc-single.pc
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

if (LIBXC_BUILD_SHARED)
	install(
		FILES ${CMAKE_CURRENT_BINARY_DIR}/libxc.pc
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
	)
endif()

if (LIBXC_BUILD_STATIC)
	install(
		FILES ${CMAKE_CURRENT_BINARY_DIR}/libxc-static.pc
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
	)
endif()

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/libxcConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	"${CMAKE_SOURCE_DIR}/cmake/libxcConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/libxcConfig.cmake"
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libxc
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/libxcConfig.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/libxcConfigVersion.cmake"
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libxc
)

export(TARGETS ${LIBXC_TARGETS} libxc-single FILE libxcTargets.cmake)

# Optional: install CMake config for find_package()
install(EXPORT libxcTargets
	FILE libxcTargets.cmake
	NAMESPACE libxc::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libxc
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

	add_library(libxc-test SHARED ${CMAKE_SOURCE_DIR}/test/libxc_test.c)
	target_compile_definitions(libxc-test PRIVATE XSTDAPI_BUILD)
	target_include_directories(libxc-test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/libxc)
	if(WIN32)
		target_link_options(libxc-test PRIVATE "LINKER:--output-def=${CMAKE_CURRENT_BINARY_DIR}/libxc-test.def")
	endif()

	function(add_test_executable _NAME )
		add_executable(${_NAME} ${ARGN})
		target_include_directories(${_NAME} PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/include/libxc
			${CMAKE_CURRENT_SOURCE_DIR}/include/libxcxx
			${CMAKE_CURRENT_SOURCE_DIR}/include
		)
		if(LIBXC_BUILD_SHARED)
			target_link_libraries(${_NAME} PRIVATE libxc)
		else()
			target_link_libraries(${_NAME} PRIVATE libxc-static)
		endif()
		#target_link_options(${_NAME} PRIVATE "LINKER:--output-def=${CMAKE_CURRENT_BINARY_DIR}/${_NAME}.def")

		if (WINE_EXECUTABLE)
			add_test(NAME ${_NAME} COMMAND ${WINE_EXECUTABLE} $<TARGET_FILE:${_NAME}>)
		else()
			add_test(NAME ${_NAME} COMMAND $<TARGET_FILE:${_NAME}>)
		endif()
	endfunction()

	# Add executables for each test
	add_test_executable(test_xxlocale  ${CMAKE_SOURCE_DIR}/test/test_xxlocale.c)
	add_test_executable(test_xlog      ${CMAKE_SOURCE_DIR}/test/test_xlog.c)
	add_test_executable(test_xstdio    ${CMAKE_SOURCE_DIR}/test/test_xstdio.c)
	add_test_executable(test_xstdlib   ${CMAKE_SOURCE_DIR}/test/test_xstdlib.c)
	add_test_executable(test_xstring   ${CMAKE_SOURCE_DIR}/test/test_xstring.c)
	add_test_executable(test_xwchar    ${CMAKE_SOURCE_DIR}/test/test_xwchar.c)
	add_test_executable(test_xctype    ${CMAKE_SOURCE_DIR}/test/test_xctype.c)
	add_test_executable(test_xwctype   ${CMAKE_SOURCE_DIR}/test/test_xwctype.c)
	add_test_executable(test_xstat     ${CMAKE_SOURCE_DIR}/test/test_xstat.c)
	add_test_executable(test_xmemory   ${CMAKE_SOURCE_DIR}/test/test_xmemory.c)
	add_test_executable(test_xuchar    ${CMAKE_SOURCE_DIR}/test/test_xuchar.c)
	add_test_executable(test_xuchar++  ${CMAKE_SOURCE_DIR}/test/test_xuchar.cpp)

	add_test_executable(test_xstring++ ${CMAKE_SOURCE_DIR}/test/test_xstring.cpp)
	add_test_executable(test_xstring_c++26 ${CMAKE_SOURCE_DIR}/test/test_xstring.cpp)
	set_target_properties(test_xstring_c++26 PROPERTIES
		CXX_STANDARD 26
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
	)

	add_test_executable(test_xstring_view++ ${CMAKE_SOURCE_DIR}/test/test_xstring_view.cpp)
	set_target_properties(test_xstring_view++ PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
	)

	add_test_executable(test_xstring_view_c++26 ${CMAKE_SOURCE_DIR}/test/test_xstring_view.cpp)
	set_target_properties(test_xstring_view_c++26 PROPERTIES
		CXX_STANDARD 26
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
	)

	add_test_executable(test_xdlfcn    ${CMAKE_SOURCE_DIR}/test/test_xdlfcn.c)
	target_link_libraries(test_xdlfcn PRIVATE libxc-test)
	if(NOT WIN32)
		target_link_options(test_xdlfcn PRIVATE -rdynamic)
	endif()

	add_test_executable(test_xmap      ${CMAKE_SOURCE_DIR}/test/test_xmap.cc)
endif()
