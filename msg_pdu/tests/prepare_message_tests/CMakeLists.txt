add_executable(prepare_message_tests
    prepare_message_tests.cpp
)
target_link_libraries(prepare_message_tests PUBLIC ${MSG_PDU_LIB_NAME})
target_include_directories(prepare_message_tests PUBLIC ${MSG_PDU_INCLUDE_DIR})
