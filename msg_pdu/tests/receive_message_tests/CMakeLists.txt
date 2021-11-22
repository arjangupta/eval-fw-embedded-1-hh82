add_executable(receive_message_tests
    receive_message_tests.cpp
)
target_link_libraries(receive_message_tests 
    PUBLIC ${MSG_PDU_LIB_NAME}
    PRIVATE gtest_main)
target_include_directories(receive_message_tests PUBLIC ${MSG_PDU_INCLUDE_DIR})
add_test(NAME receive_message_tests COMMAND receive_message_tests)
