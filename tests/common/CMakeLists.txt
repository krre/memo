add_library(test_common STATIC
    TestSettings.h TestSettings.cpp
    TestHelper.h
)

target_link_libraries(test_common PRIVATE common)
target_include_directories(test_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
