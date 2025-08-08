# Auto-detect vcpkg toolchain file
# This uses vcpkg's debug output to find the actual installation path

find_program(VCPKG_EXECUTABLE vcpkg)

if(VCPKG_EXECUTABLE)
    # Get vcpkg root by asking vcpkg itself
    execute_process(
        COMMAND "${VCPKG_EXECUTABLE}" version --debug
        OUTPUT_VARIABLE VCPKG_DEBUG_OUTPUT
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    # Extract vcpkg root from debug output (looks for /path/to/vcpkg/vcpkg-bundle.json)
    string(REGEX MATCH "(/[^\\s]+)/vcpkg-bundle\\.json" VCPKG_ROOT_MATCH "${VCPKG_DEBUG_OUTPUT}")
    if(CMAKE_MATCH_1)
        set(VCPKG_ROOT "${CMAKE_MATCH_1}")
        set(VCPKG_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        
        if(EXISTS "${VCPKG_TOOLCHAIN_FILE}")
            include("${VCPKG_TOOLCHAIN_FILE}")
        else()
            message(FATAL_ERROR "vcpkg toolchain file not found at: ${VCPKG_TOOLCHAIN_FILE}")
        endif()
    else()
        message(FATAL_ERROR "Could not determine vcpkg root from debug output")
    endif()
else()
    message(FATAL_ERROR "vcpkg not found in PATH. Please install vcpkg and add it to PATH.")
endif()
