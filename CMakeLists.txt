if (WIN32)
    cmake_minimum_required(VERSION 3.12)
elseif (APPLE)
    cmake_minimum_required(VERSION 3.17)
elseif (UNIX)
    cmake_minimum_required(VERSION 3.17)
endif ()
project(PaddleOcrJni)

if (NOT DEFINED OCR_OUTPUT)
    set(OCR_OUTPUT "BIN")
    message(STATUS "No OCR_OUTPUT, defaulting to BIN")
endif ()
option(OCR_BENCHMARK "build benchmark" ON)
set(OCR_BENCHMARK ON)
#set(OCR_OUTPUT "BIN")

set(CMAKE_CXX_STANDARD 11)
add_definitions(-DUNICODE -D_UNICODE)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-Wall -g -O0")
else ()
    add_definitions("-Wall")
endif ()

# OpenMP flags for MACOS
if (APPLE)
    message("YOU SHOULD NEVER SEE THIS ON CentOS!!")
    if (CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(OpenMP_C "${CMAKE_C_COMPILER}")
        set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include")
        set(OpenMP_C_LIB_NAMES "omp")
        set(OpenMP_omp_LIBRARY ${OpenMP_C_LIB_NAMES})
    endif ()
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(OpenMP_CXX "${CMAKE_CXX_COMPILER}")
        set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include")
        set(OpenMP_CXX_LIB_NAMES "omp")
        set(OpenMP_omp_LIBRARY ${OpenMP_C_LIB_NAMES})
    endif ()
    link_directories("/usr/local/opt/libomp/lib")
endif ()


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -z noexecstack")

# OpenMP
# find_package(OpenMP REQUIRED)
# if (OPENMP_FOUND)
#    message("OPENMP FOUND")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -z noexecstack")
# else ()
#    message(FATAL_ERROR "OpenMP Not Found!")
# endif ()

# OpenCV
set(BUILD_SHARED_LIBS false)
include(${CMAKE_CURRENT_SOURCE_DIR}/opencv-static/OpenCVWrapperConfig.cmake)
find_package(OpenCV REQUIRED)
if (OpenCV_FOUND)
    message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")
    message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
else ()
    message(FATAL_ERROR "opencv Not Found!")
endif (OpenCV_FOUND)

# PaddleInference
include(${CMAKE_CURRENT_SOURCE_DIR}/paddle_inference_install_dir/PaddleInferenceWrapper.cmake)
find_package(PaddleInference REQUIRED)
if (PaddleInference_FOUND)
    message(STATUS "PaddleInference_LIBS: ${PaddleInference_LIBS}")
    message(STATUS "PaddleInference_INCLUDE_DIRS: ${PaddleInference_INCLUDE_DIRS}")
else ()
    message(FATAL_ERROR "onnxruntime Not Found!")
endif (PaddleInference_FOUND)

# project include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# source
file(GLOB OCR_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(OCR_COMPILE_CODE ${OCR_SRC})

# JNI
if (OCR_OUTPUT STREQUAL "JNI")
    # set(JAVA_AWT_LIBRARY "$ENV{JAVA_HOME}/lib/libjawt.so")
    set(JAVA_JVM_LIBRARY "$ENV{JAVA_HOME}/lib/server/libjvm.so")

    set(JAVA_INCLUDE_PATH "$ENV{JAVA_HOME}/include")

    if (APPLE)
        set(JAVA_INCLUDE_PATH2 "$ENV{JAVA_HOME}/include/darwin")
    elseif (WIN32)
         set(JAVA_INCLUDE_PATH2 "$ENV{JAVA_HOME}/include/win32")
    elseif (UNIX)
         set(JAVA_INCLUDE_PATH2 "$ENV{JAVA_HOME}/include/linux")
    endif ()
    message(STATUS "JAVA_INCLUDE_PATH2: ${JAVA_INCLUDE_PATH2}")

    set(JAVA_AWT_INCLUDE_PATH "$ENV{JAVA_HOME}/include")
    find_package(JNI REQUIRED)
    if (JNI_FOUND)
        message("JNI FOUND")
        message(STATUS "JNI_LIBS: ${JNI_LIBS}")
        message(STATUS "JNI_INCLUDE_DIRS: ${JNI_INCLUDE_DIRS}")
        include_directories(${JNI_INCLUDE_DIRS})
    else ()
        message(FATAL_ERROR "JNI Not Found!")
    endif ()
endif ()

if (OCR_OUTPUT STREQUAL "JNI") # JNI
    add_library(PaddleOcrJni SHARED ${OCR_COMPILE_CODE})
    target_compile_definitions(PaddleOcrJni PRIVATE __JNI__)
    target_link_libraries(PaddleOcrJni ${OpenCV_LIBS} ${PaddleInference_LIBS} ${JNI_LIBS})
endif ()

install(TARGETS PaddleOcrJni EXPORT PaddleOcrJni)

