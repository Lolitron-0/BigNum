cmake_minimum_required(VERSION 3.25)
project(BigNum CXX)  
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3")
set(CMAKE_CXX_FLAGS --coverage)
# add_compile_options(-fsanitize=undefined,address)
# add_link_options(-fsanitize=undefined,address)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(SOURCES
	src/BigNum.cpp
)

enable_testing()

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_definitions(${PROJECT_NAME} PRIVATE 
	PROJECT_NAME=${PROJECT_NAME}
	#TEST_BN_OPS
)

configure_file(tests/pi_string pi_string COPYONLY)
configure_file(tests/coeff_string coeff_string COPYONLY)

add_executable(${PROJECT_NAME}_pi tests/pi_eval.cpp)
target_link_libraries(${PROJECT_NAME}_pi PRIVATE ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}_pi PRIVATE include)

# Tests
add_executable(${PROJECT_NAME}_test tests/tests.cpp)
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})

target_link_libraries(
	${PROJECT_NAME}_test
	PRIVATE
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
