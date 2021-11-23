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

TEST(receive_message_tests, happy_path_test1)
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

    // Generate the source address
    const uint16_t destination_address = get_my_addr();
    uint16_t source_address = getRandomByte(); 
    while (destination_address == source_address)
    {
        source_address = getRandomTwoBytes();
    }
    std::cout << "Source and destination addresses are: " << source_address << " " << destination_address << std::endl;

    // Allocate input buffer
    const int buffer_alloc_size = 1+1+2+2+1+message_body_length+2;
    std::cout << "buffer alloc size is " << buffer_alloc_size << std::endl;

    // Populate input buffer
    uint8_t input_buffer[buffer_alloc_size] = {
        Msg_PDU_PREAMBLE0,
        Msg_PDU_PREAMBLE1,
        // Big endian storage (THESE ARE NOT BITWISE ON PURPOSE - I do not want to test the code with the same code inside receiveMessage()):
        *(((uint8_t*)&source_address)+1),
        *((uint8_t*)&source_address),
        *(((uint8_t*)&destination_address)+1),
        *((uint8_t*)&destination_address),
        message_body_length
    };
    std::cout << "Printing the first seven bytes of expected buffer: " << std::endl;
    for (size_t i = 0; i < 7; i++)
    {
        std::cout << "input_buffer[" << i << "] : " << (int)input_buffer[i] << std::endl;
    }
    memcpy((input_buffer+1+1+2+2+1), pdu_body_vector.data(), message_body_length);
    // Get check sum
    const uint16_t checksum = calculate_checksum(pdu_body_vector.data(), message_body_length);
    input_buffer[1+1+2+2+1+message_body_length] = *(((uint8_t*)&checksum)+1);
    input_buffer[1+1+2+2+1+message_body_length+1] = *((uint8_t*)&checksum);

    // Call the function for test
    Msg_PDU resultant_pdu[1] = { 0 };
    receiveMessage(input_buffer, buffer_alloc_size, resultant_pdu);

    // Form expected pdu
    Msg_PDU expected_pdu;
    expected_pdu.dest_addr = destination_address;
    expected_pdu.src_addr = source_address;
    expected_pdu.body_len = message_body_length;
    memcpy(expected_pdu.body, pdu_body_vector.data(), message_body_length);

    EXPECT_EQ(expected_pdu.dest_addr, resultant_pdu[0].dest_addr);
    EXPECT_EQ(expected_pdu.src_addr, resultant_pdu[0].src_addr);
    EXPECT_EQ(expected_pdu.body_len, resultant_pdu[0].body_len);
    EXPECT_EQ(memcmp(expected_pdu.body, resultant_pdu[0].body, expected_pdu.body_len), 0);
}

} // namespace testing_msg_pdu
