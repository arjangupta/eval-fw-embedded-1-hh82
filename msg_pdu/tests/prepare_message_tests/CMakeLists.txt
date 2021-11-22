add_executable(prepare_message_tests
    prepare_message_tests.cpp
)
target_link_libraries(prepare_message_tests 
    PUBLIC ${MSG_PDU_LIB_NAME}
    PRIVATE gtest_main)
target_include_directories(prepare_message_tests PUBLIC ${MSG_PDU_INCLUDE_DIR})
add_test(NAME prepare_message_tests COMMAND prepare_message_tests)
