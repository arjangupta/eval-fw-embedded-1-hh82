extern "C"
{
#include "msg_pdu.h"
}
#include <gtest/gtest.h>
#include <iostream>

namespace testing_msg_pdu
{

const uint16_t getRandomTwoBytes()
{
    srand(time(NULL));
    return rand() % 65536;
}

const uint8_t getRandomByte()
{
    srand(time(NULL));
    return rand() % 256;
}

TEST(prepare_message_tests, happy_path_test1)
{
    // Form the message body
    const uint8_t message_body_length = getRandomByte();
    std::cout << "Message body length generated as: " << (int)message_body_length << std::endl;
    std::vector<uint8_t> pdu_body_vector;
    pdu_body_vector.reserve(message_body_length);
    for (size_t i = 0; i < message_body_length; i++)
    {
        pdu_body_vector.push_back(getRandomByte());
    }

    // Destination addr 453, source addr 22, body len 12,
    const uint16_t destination_address = getRandomTwoBytes();
    uint16_t source_address = getRandomTwoBytes();
    while (destination_address == source_address)
    {
        source_address = getRandomTwoBytes();
    }
    std::cout << "Source and destination addresses are: " << source_address << " " << destination_address << std::endl;
    
    // Initialize pdu
    Msg_PDU test_pdu1;
    test_pdu1.dest_addr = destination_address;
    test_pdu1.src_addr = source_address;
    test_pdu1.body_len = message_body_length;
    memcpy(test_pdu1.body, pdu_body_vector.data(), message_body_length);

    // Call function for test
    const int buffer_alloc_size = 1+1+2+2+1+message_body_length+2;
    std::cout << "buffer alloc size is " << buffer_alloc_size << std::endl;
    uint8_t* output_buffer = (uint8_t*)malloc(buffer_alloc_size);
    prepareMessage(&test_pdu1, output_buffer, buffer_alloc_size);

    // Create expectation buffer
    uint8_t expected_buffer[buffer_alloc_size] = {
        Msg_PDU_PREAMBLE0,
        Msg_PDU_PREAMBLE1,
        // Big endian storage:
        *(((uint8_t*)&source_address)+1),
        *((uint8_t*)&source_address),
        *(((uint8_t*)&destination_address)+1),
        *((uint8_t*)&destination_address),
        message_body_length
    };
    std::cout << "Printing the first seven bytes of expected buffer: " << std::endl;
    for (size_t i = 0; i < 7; i++)
    {
        std::cout << "expected_buffer[" << i << "] : " << (int)expected_buffer[i] << std::endl;
    }
    memcpy((expected_buffer+1+1+2+2+1), pdu_body_vector.data(), message_body_length);
    expected_buffer[1+1+2+2+1+message_body_length] = calculate_checksum(pdu_body_vector.data(), message_body_length);

    EXPECT_EQ(memcmp(expected_buffer, output_buffer, buffer_alloc_size), 0);

    // Free memory
    delete output_buffer;
}

} // namespace testing_msg_pdu
