cmake_minimum_required (VERSION 3.20)
project(OCRTest)

set(CMAKE_CXX_STANDARD 11)


add_subdirectory(test testBin)


include_directories(${CMAKE_SOURCE_DIR}/include)

include(${CMAKE_SOURCE_DIR}/build/conanbuildinfo.cmake)

conan_basic_setup()

add_executable(ocrimg ocrimg.cpp)
add_executable(ocrlatex ocrlatex.cpp)



target_link_libraries(
        ocrimg
        tencentcloud-sdk-cpp-ocr
        tencentcloud-sdk-cpp-core
        curl
        pthread
)


target_link_libraries(
        ocrlatex
        tencentcloud-sdk-cpp-ocr
        tencentcloud-sdk-cpp-core
        curl
        pthread
)


target_link_libraries(ocrimg ${CONAN_LIBS})
target_link_libraries(ocrlatex ${CONAN_LIBS})


install(FILES test/test_txt.png DESTINATION ${CMAKE_SOURCE_DIR}/build/bin )
install(FILES test/test_latex.png DESTINATION ${CMAKE_SOURCE_DIR}/build/bin )