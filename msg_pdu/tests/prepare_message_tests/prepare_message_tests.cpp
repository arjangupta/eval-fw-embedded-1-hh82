#include "msg_pdu.h"
#include <gtest/gtest.h>
#include <iostream>

namespace testing_msg_pdu
{

TEST(prepare_message_tests, happy_path_test1)
{
    // Form the message body
    const uint8_t message_body_length = rand() % 256;
    std::cout << "Message body length generated as: " << message_body_length << std::endl;
    std::vector<uint8_t> pdu_body_vector;
    pdu_body_vector.reserve(message_body_length);
    for (size_t i = 0; i < message_body_length; i++)
    {
        uint8_t random_byte = rand() % 256; // Some value 0-255
    }

    // Destination addr 453, source addr 22, body len 12, 
    Msg_PDU pdu1 = { 453, 22, message_body_length,  };
}

} // namespace testing_msg_pdu
