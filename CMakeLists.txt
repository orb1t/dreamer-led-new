# !!! WARNING !!! AUTO-GENERATED FILE, PLEASE DO NOT MODIFY IT AND USE
# https://docs.platformio.org/page/projectconf/section_env_build.html#build-flags
#
# If you need to override existing CMake configuration or add extra,
# please create `CMakeListsUser.txt` in the root of project.
# The `CMakeListsUser.txt` will not be overwritten by PlatformIO.

cmake_minimum_required(VERSION 3.13)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project("201122-212358-esp32dev" C CXX)

include(CMakeListsPrivate.txt)
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/effects")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/games")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/media")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/morphingClock")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/system")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src")
include_directories("${CMAKE_CURRENT_LIST_DIR}/lib/TJpg_Decoder/src")

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeListsUser.txt)
include(CMakeListsUser.txt)
endif()

add_custom_target(
    Production ALL
    COMMAND platformio -c clion run "$<$<NOT:$<CONFIG:All>>:-e${CMAKE_BUILD_TYPE}>"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    Debug ALL
    COMMAND platformio -c clion run --target debug "$<$<NOT:$<CONFIG:All>>:-e${CMAKE_BUILD_TYPE}>"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

#add_executable(Z_DUMMY_TARGET ${SRC_LIST} src/system/FreeRTOSTimer.cpp src/system/FreeRTOSTimer.h src/games/Snake.h src/effects/defs.h src/media/GifDrawer.cpp src/media/helper.h)
#add_executable(Z_DUMMY_TARGET ${SRC_LIST} src/effects/PatternFire.h src/Async_ConfigOnDoubleReset.h)
#add_executable(Z_DUMMY_TARGET ${SRC_LIST} src/ModeRuner.h src/jsonHelper.h)
#add_executable(Z_DUMMY_TARGET ${SRC_LIST} src/system/WiFiPortal.h src/globalModes.h)
#add_executable(Z_DUMMY_TARGET ${SRC_LIST} src/games/helper.h)
add_executable(Z_DUMMY_TARGET ${SRC_LIST})
