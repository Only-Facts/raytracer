cmake_minimum_required(VERSION 3.20)
set(TARGET raytracer)
project(${TARGET} VERSION 1.0 LANGUAGES CXX)

# =========================
# Compilateur & Standard
# =========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}) # TARGET destination
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # Moderne compilation -fPIE
add_definitions(-DFORCED_CXX_STANDARD=14) # Used for the attribute definition
# Unknow flag -> add_compile_options(-fno-gnu-unique) # Ensure unload from dclose properly

# =========================
# Requirement
# =========================
find_package(Python3 REQUIRED) # Try to find python for the dependencies
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED libconfig++)

# =========================
# Warnings
# =========================
add_compile_options(
    -W
    -Wall
    -Wextra
    -Wpedantic
    -Wunused-parameter
    -Wshadow
    -Wuninitialized
    -Wnan-infinity-disabled
)

# Sanitizer options
#add_compile_options(
#    -g -ggdb3
#    -fno-omit-frame-pointer
#    -fsanitize=address
#)
#add_link_options(-fsanitize=address)
add_compile_options(
    -g -O0 -fno-omit-frame-pointer
)

# =========================
# Sources
# =========================
set(SRC
    # Global
    src/raytracer/main.cpp
    src/raytracer/Raytracer.cpp
    src/raytracer/RaytracerInit.cpp
    src/raytracer/DynamicLibrary.cpp

    # Objects
    src/raytracer/objects/AObject.cpp

    # Rays
    src/raytracer/rays/ARay.cpp
    src/raytracer/rays/Ray.cpp
    src/raytracer/rays/LightRay.cpp

    # Camera (used for the viewer mode)
    src/raytracer/cameras/ACamera.cpp
    src/raytracer/cameras/viewer/Viewer.cpp
)
add_executable(${TARGET} ${SRC})
#add_library(${TARGET} STATIC ${SRC})
#add_library(${TARGET} SHARED ${SRC})

set(SRC_LIBS
)

# =========================
# Cameras
# =========================
set(SRC_CAMERAS
    src/raytracer/rays/ARay.cpp
    src/raytracer/rays/Ray.cpp
    src/raytracer/cameras/lib.cpp
    src/raytracer/cameras/ACamera.cpp
    src/raytracer/objects/AObject.cpp
)

set(SRC_VIEWER
    src/raytracer/cameras/viewer/lib.cpp
    src/raytracer/cameras/viewer/Viewer.cpp
)
add_library(camera_viewer SHARED ${SRC_LIBS} ${SRC_CAMERAS} ${SRC_VIEWER})

set(SRC_CAMERA2D
    src/raytracer/cameras/camera2D/lib.cpp
    src/raytracer/cameras/camera2D/Camera2D.cpp
)
add_library(camera_camera2D SHARED ${SRC_LIBS} ${SRC_CAMERAS} ${SRC_CAMERA2D})

set(SRC_CAMERAFOV
    src/raytracer/cameras/cameraFOV/lib.cpp
    src/raytracer/cameras/cameraFOV/CameraFOV.cpp
)
add_library(camera_cameraFOV SHARED ${SRC_LIBS} ${SRC_CAMERAS} ${SRC_CAMERAFOV})

# =========================
# Lights
# =========================
set(SRC_LIGHTS
    src/raytracer/rays/ARay.cpp
    src/raytracer/rays/LightRay.cpp
    src/raytracer/lights/lib.cpp
    src/raytracer/lights/ALight.cpp
    src/raytracer/objects/AObject.cpp
)

set(SRC_POINT
    src/raytracer/lights/point/lib.cpp
    src/raytracer/lights/point/Point.cpp
)
add_library(light_point SHARED ${SRC_LIBS} ${SRC_LIGHTS} ${SRC_POINT})

set(SRC_DIRECTIONAL
    src/raytracer/lights/directional/lib.cpp
    src/raytracer/lights/directional/Directional.cpp
)
add_library(light_directional SHARED ${SRC_LIBS} ${SRC_LIGHTS} ${SRC_DIRECTIONAL})

# =========================
# Objects
# =========================
set(SRC_OBJECTS
    src/raytracer/DynamicLibrary.cpp
    src/raytracer/RaytracerInit.cpp
    src/raytracer/objects/lib.cpp
    src/raytracer/objects/AObject.cpp
)

set(SRC_OBJECT
    src/raytracer/objects/object/lib.cpp
    src/raytracer/objects/object/Object.cpp
)
add_library(object_object SHARED ${SRC_LIBS} ${SRC_OBJECTS} ${SRC_OBJECT})

set(SRC_POINT
    src/raytracer/objects/point/lib.cpp
    src/raytracer/objects/point/Point.cpp
)
add_library(object_point SHARED ${SRC_LIBS} ${SRC_OBJECTS} ${SRC_POINT})

set(SRC_PLANE
    src/raytracer/objects/plane/lib.cpp
    src/raytracer/objects/plane/Plane.cpp
)
add_library(object_plane SHARED ${SRC_LIBS} ${SRC_OBJECTS} ${SRC_PLANE})

set(SRC_SPHERE
    src/raytracer/objects/sphere/lib.cpp
    src/raytracer/objects/sphere/Sphere.cpp
)
add_library(object_sphere SHARED ${SRC_LIBS} ${SRC_OBJECTS} ${SRC_SPHERE})

set(SRC_CYLINDER
    src/raytracer/objects/cylinder/lib.cpp
    src/raytracer/objects/cylinder/Cylinder.cpp
)
add_library(object_cylinder SHARED ${SRC_LIBS} ${SRC_OBJECTS} ${SRC_CYLINDER})

set(SRC_CONE
    src/raytracer/objects/cone/lib.cpp
    src/raytracer/objects/cone/Cone.cpp
)
add_library(object_cone SHARED ${SRC_LIBS} ${SRC_OBJECTS} ${SRC_CONE})

# =========================
# Materials
# =========================
set(SRC_MATERIALS
    src/raytracer/materials/lib.cpp
)

set(SRC_DEFAULT
    src/raytracer/materials/default/lib.cpp
    src/raytracer/materials/default/Default.cpp
)
add_library(material_default SHARED ${SRC_LIBS} ${SRC_MATERIALS} ${SRC_DEFAULT})

# =========================
# Compilation parameters
# =========================
set_target_properties(
    camera_viewer
    camera_camera2D
    camera_cameraFOV
    PROPERTIES
        PREFIX ""
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/cameras
)
set_target_properties(
    light_point
    light_directional
    PROPERTIES
        PREFIX ""
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/lights
)
set_target_properties(
    object_object
    object_point
    object_plane
    object_sphere
    object_cylinder
    object_cone
    PROPERTIES
        PREFIX ""
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/objects
)
set_target_properties(
    material_default
    PROPERTIES
        PREFIX ""
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/materials
)

# =========================
# Dependencies
# =========================
add_custom_target(generated_exception_header ALL
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/cmake/scripts/generate_exception_header.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_dependencies(${TARGET} generated_exception_header)
foreach(target
    camera_viewer
    camera_camera2D
    camera_cameraFOV
    light_point
    light_directional
    object_object
    object_point
    object_plane
    object_sphere
    object_cylinder
    object_cone
    material_default
)
    add_dependencies(${target} generated_exception_header)
endforeach()

# =========================
# Includes
# =========================
# Global includes
target_include_directories(${TARGET} PRIVATE include)
target_link_directories(${TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/cmake/libs/)
target_link_libraries(${TARGET} PRIVATE utils)
target_include_directories(${TARGET} PRIVATE ${LIBCONFIG_INCLUDE_DIRS})
target_link_libraries(${TARGET} PRIVATE ${LIBCONFIG_LIBRARIES})
target_link_libraries(${TARGET} PRIVATE sfml-graphics sfml-window sfml-system)
foreach(target
    camera_viewer
    camera_camera2D
    camera_cameraFOV
    light_point
    light_directional
    object_object
    object_point
    object_plane
    object_sphere
    object_cylinder
    object_cone
    material_default
)
    target_include_directories(${target} PRIVATE include)
    target_link_directories(${target} PRIVATE ${CMAKE_SOURCE_DIR}/cmake/libs/)
    target_link_libraries(${target} PRIVATE utils)
    target_include_directories(${target} PRIVATE ${LIBCONFIG_INCLUDE_DIRS})
    target_link_libraries(${target} PRIVATE ${LIBCONFIG_LIBRARIES})
    target_link_libraries(${target} PRIVATE sfml-graphics sfml-window sfml-system)
endforeach()

# =========================
# Modes
# =========================
target_compile_options(${TARGET} PRIVATE
    $<$<CONFIG:Debug>:-g -ggdb3>
    $<$<CONFIG:Optimized>:-O3 -ffast-math -march=native -funroll-loops -pipe -DNDEBUG>
    # -flto
)

# =========================
# Utils
# =========================
add_custom_target(get_unregistered_files
    COMMAND bash ${CMAKE_SOURCE_DIR}/cmake/scripts/check_files.sh unregistered ${SRC}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
