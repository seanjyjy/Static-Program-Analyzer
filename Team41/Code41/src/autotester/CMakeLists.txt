file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(autotester ${srcs})
target_link_libraries(autotester spa ${AUTOTESTER_LIBRARIES})

if (NOT WIN32)
    target_link_libraries(autotester pthread)
endif()

file(GLOB SIMPLE_TESTS "${PROJECT_SOURCE_DIR}/tests/*_source.txt")
foreach(SIMPLE_SRC ${SIMPLE_TESTS})
    string(REGEX REPLACE "_source.txt$" "_queries.txt" PQL_QUERIES ${SIMPLE_SRC})
    string(REGEX MATCH "([A-Za-z0-9_]*)_source.txt$" TEST_NAME "${SIMPLE_SRC}")
    string(REGEX REPLACE "_source.txt$" "" TEST_NAME ${TEST_NAME})
    message(STATUS "Test ${TEST_NAME} source=${SIMPLE_SRC}, query=${PQL_QUERIES}")
    add_test("autotester-${TEST_NAME}" autotester ${SIMPLE_SRC} ${PQL_QUERIES} -)
    set_tests_properties("autotester-${TEST_NAME}" PROPERTIES
            FAIL_REGULAR_EXPRESSION "Missing: ;Additional: ")
endforeach()