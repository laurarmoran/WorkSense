
cmake_minimum_required(VERSION 3.20)

project(WorkSense
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(worksense
    src/main.cpp
    src/Alarm.cpp
    src/Device.cpp
    src/DeviceManager.cpp
)

target_include_directories(worksense PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

if(MINGW)
    get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
    foreach(runtime_dll IN ITEMS
        libgcc_s_seh-1.dll
        libstdc++-6.dll
        libwinpthread-1.dll
    )
        add_custom_command(TARGET worksense POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${MINGW_BIN_DIR}/${runtime_dll}"
                "$<TARGET_FILE_DIR:worksense>"
        )
    endforeach()
endif()
