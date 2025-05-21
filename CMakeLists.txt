cmake_minimum_required(VERSION 3.16)
project(raytracer LANGUAGES CXX)

# === C++ standard ===
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


# === Find libconfig++ via pkg-config ===
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG++ REQUIRED libconfig++)
include_directories(${LIBCONFIG++_INCLUDE_DIRS})

# === Optional SFML viewer ===
option(BUILD_SFML_VIEWER "Build interactive SFML viewer" ON)
if (BUILD_SFML_VIEWER)
    find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
endif()

# === Output locations ===
# Executable at project root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
# Core library in build/
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# === 1) Core shared library ===
#   All src/*.cpp except src/Plugins/*
file(GLOB_RECURSE CORE_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)
list(FILTER CORE_SOURCES EXCLUDE REGEX ".*/Plugins/.*")

add_library(raytracer_core SHARED ${CORE_SOURCES})

target_include_directories(raytracer_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(raytracer_core
    PUBLIC
        ${LIBCONFIG++_LIBRARIES}
)

if (BUILD_SFML_VIEWER)
    target_link_libraries(raytracer_core
        PUBLIC
            sfml-system
            sfml-window
            sfml-graphics
    )
endif()

set_target_properties(raytracer_core PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    LIBRARY_OUTPUT_DIRECTORY       ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    BUILD_WITH_INSTALL_RPATH       TRUE
    INSTALL_RPATH                  "$ORIGIN/build"
)

# === 2) Main executable ===
add_executable(raytracer src/main.cpp)

target_link_libraries(raytracer
    PRIVATE
        raytracer_core
)

if (BUILD_SFML_VIEWER)
    target_link_libraries(raytracer
        PRIVATE
            sfml-system
            sfml-window
            sfml-graphics
    )
endif()

set_target_properties(raytracer PROPERTIES
    INSTALL_RPATH "$ORIGIN/build"
)

# === 3) Plugins (.so) ===
#   Output to <repo_root>/plugins
set(PLUGIN_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/plugins)
file(MAKE_DIRECTORY ${PLUGIN_OUTPUT_DIR})

file(GLOB_RECURSE PLUGIN_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/Plugins/*.cpp
)

foreach(srcfile ${PLUGIN_SOURCES})
    get_filename_component(name ${srcfile} NAME_WE)

    add_library(${name} SHARED ${srcfile})

    target_include_directories(${name}
        PRIVATE
            ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(${name}
        PRIVATE
            raytracer_core
    )

    set_target_properties(${name} PROPERTIES
        PREFIX ""
        SUFFIX ".so"
        POSITION_INDEPENDENT_CODE ON
        LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR}
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "\$ORIGIN/../build"
    )
endforeach()


# === 4) Unit tests with Criterion ===
pkg_check_modules(CRITERION criterion)

if(CRITERION_FOUND)
    file(GLOB_RECURSE TEST_SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
    list(FILTER TEST_SRC_FILES EXCLUDE REGEX ".*/Plugins/.*")
    list(FILTER TEST_SRC_FILES EXCLUDE REGEX ".*/main.cpp$")
    
    file(GLOB TEST_FILES CONFIGURE_DEPENDS tests/*.cpp)
    
    if(TEST_FILES)
        add_executable(tests_run ${TEST_FILES} ${TEST_SRC_FILES})
        target_include_directories(tests_run PRIVATE include)
        target_include_directories(tests_run SYSTEM PRIVATE 
            ${LIBCONFIG++_INCLUDE_DIRS} 
            ${CRITERION_INCLUDE_DIRS})
        target_link_libraries(tests_run PRIVATE 
            ${LIBCONFIG++_LIBRARIES} 
            ${CRITERION_LIBRARIES})
            
        set_target_properties(tests_run PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
            
        if (BUILD_SFML_VIEWER)
            target_link_libraries(tests_run PRIVATE 
                sfml-system sfml-window sfml-graphics)
        endif()
    endif()
endif()
