set(CMAKE_MIN_REQ_VERSION 3.14..3.29)
cmake_minimum_required(VERSION ${CMAKE_MIN_REQ_VERSION})

project(
    FBXtoM2
    VERSION 0.1
    DESCRIPTION "An example project with CMake"
    LANGUAGES CXX)

#/Users/abai/Documents/Freelance/fbxm2/manual/fbx_sdk/2020.3.2/include/fbxsdk/core/arch/fbxstdcompliant.h:52:96 warning: 'vsprintf' is deprecated

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here
    set(CMAKE_CXX_STANDARD 14)
    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

find_package(LibXml2 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Iconv REQUIRED)

add_library(fbxsdk STATIC IMPORTED)
set_target_properties(fbxsdk PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/fbx_sdk/2020.3.2/lib/clang/release/libfbxsdk.a
    #INTERFACE_COMPILE_DEFINITIONS "SOME_FEATURE"
)

add_executable(
    fbxtom2
    main.cpp
    m2writer.cpp
    fbx_sdk/2020.3.2/samples/Common/Common.h
    fbx_sdk/2020.3.2/samples/Common/Common.cxx    
)

SET_TARGET_PROPERTIES(fbxtom2 PROPERTIES LINK_FLAGS 
    "-framework CoreFoundation -framework SystemConfiguration ${FBX_LINKER_FLAGS}")

target_include_directories(fbxtom2 PRIVATE 
    fbx_sdk/2020.3.2/samples/Common 
    fbx_sdk/2020.3.2/include
    ${LIBXML2_INCLUDE_DIR})

target_link_libraries(fbxtom2 PRIVATE 
    fbxsdk
    ${LIBXML2_LIBRARIES}
    ZLIB::ZLIB
    Iconv::Iconv)