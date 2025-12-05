file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(tests ${TEST_SOURCES})
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tests
PRIVATE
Catch2::Catch2WithMain
        threepp::threepp
)

include(CTest)
include(Catch)
catch_discover_tests(tests)
