# CMakeList.txt : CMakeLists for UltReality::Rendering::DisplayTarget

# CMake configuration and settings ****************************************************************
#**************************************************************************************************

cmake_minimum_required(VERSION 3.25)

project(DISPLAY_TARGET VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define custom configuration types for Debug, Release, and Release-Dev
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "List of supported build modes." FORCE)

# Define compiler definitions to create OS specific code segments
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_compile_definitions(TARGET_OS=Windows _WIN_TARGET)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	add_compile_definitions(TARGET_OS=Linux _LINUX_TARGET)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin") #MacOS
	add_compile_definitions(TARGET_OS=MacOS _MACOS_TARGET)
endif()

# Make sure CMake exports its compile commands so things like IntelliSense can detect them in editor
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Create options that are dependent on this project being top level
option(DISPLAY_TARGET_INSTALL "Enable project install" ${PROJECT_IS_TOP_LEVEL})

# End CMake configuration and settings ************************************************************
#**************************************************************************************************

# Target Creation *********************************************************************************
#**************************************************************************************************

# Create list of all source files to be used in this target
file(GLOB_RECURSE DisplayTarget_SOURCE "${CMAKE_SOURCE_DIR}/src/*.cpp")

if(DisplayTarget_SOURCE)
# Create the Display Target Interface library
add_library(DisplayTarget STATIC ${DisplayTarget_SOURCE})

target_include_directories(DisplayTarget PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
 	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> # This is used when the library is installed
)

set_target_properties(DisplayTarget PROPERTIES 
	VERSION ${PROJECT_VERSION} 
	SOVERSION ${PROJECT_VERSION_MAJOR}
)

# End Target Creation *****************************************************************************
#**************************************************************************************************

# Installation and Packing Configuration **********************************************************
#**************************************************************************************************

# Install the library, header files, and config version file
if(DISPLAY_TARGET_INSTALL)
	# Install the targets
	install(
		TARGETS DisplayTarget 
		EXPORT DisplayTarget_Targets 
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)

	# Create the targets CMake file which contains the above definitions
	install(
		EXPORT DisplayTarget_Targets 
		FILE DisplayTarget_Targets.cmake 
		NAMESPACE UltReality::Rendering 
		DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/DisplayTarget/cmake
	)

	# Install the actual includes
	install(
		DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ 
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

	# Generate and install the package version config files
	include(CMakePackageConfigHelpers)
	write_basic_package_version_file(
		"DisplayTarget_ConfigVersion.cmake"
		VERSION ${PROJECT_VERSION} 
		COMPATIBILITY SameMajorVersion
	)
	configure_package_config_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/cmake_config/DisplayTarget_Config.cmake.in" 
		"${CMAKE_CURRENT_BINARY_DIR}/DisplayTarget_Config.cmake" 
		INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/DisplayTarget/cmake
	)

	# Install the CMake config files
	install(
		FILES "${CMAKE_CURRENT_BINARY_DIR}/DisplayTarget_ConfigVersion.cmake" 
		"${CMAKE_CURRENT_BINARY_DIR}/DisplayTarget_Config.cmake" 
		DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/DisplayTarget/cmake
	)

	# Define Package install paths
	set(UltReality_INCLUDEDIR_FOR_PKG_CONFIG "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")
	set(UltReality_LIBDIR_PKG_CONFIG "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

	# Create and install the package config file
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/cmake_config/DisplayTarget.pc.in" 
		"${CMAKE_CURRENT_BINARY_DIR}/DisplayTarget.pc" @ONLY
	)

	# Install the package config file
	install(
		FILES "${CMAKE_CURRENT_BINARY_DIR}/DisplayTarget.pc" 
 		DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
	)

	# A version that is often used to denote a specific build of the software, including revisions, builds, or other metadata
	set(PACKAGE_VERSION_BUILD "${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}")

	set(PACKAGE_VERSION "${PROJECT_VERSION}-${PACKAGE_VERSION_BUILD}")

	set(PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/out/package")
	file(MAKE_DIRECTORY "${PACKAGE_DIRECTORY}")

	set(CPACK_PACKAGE_NAME "DisplayTarget")
	set(CPACK_PACKAGE_VERSION "${PACKAGE_VERSION}")

	set(CPACK_PACKAGE_VENDOR "Andrew Todd")
	set(CPACK_PACKAGE_CONTACT "andrewdanieltodd@gmail.com")
	include(CPack)
	message(STATUS "PACKAGE_VERSION is: ${PACKAGE_VERSION}")
	message(STATUS "PACKAGE_FILE_NAME is: ${CPACK_PACKAGE_FILE_NAME}")
endif()
endif()
# End Installation and Packing Configuration ******************************************************
#**************************************************************************************************
