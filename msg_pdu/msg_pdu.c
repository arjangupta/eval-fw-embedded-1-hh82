#include "msg_pdu.h"

/*
 * @see msg_pdu.h for description
 */
int prepareMessage(Msg_PDU const * const pdu, uint8_t* const txBuf, size_t const bufSize)
{
  // Validity checks
  if (NULL == pdu)
  {
    return ERR_NULL_PDU;
  }
  else if (get_my_addr() != pdu->src_addr)
  {
    return ERR_WRONG_SRC;
  }
  else if (get_my_addr() == pdu->dest_addr)
  {
    return ERR_WRONG_DST;
  }

  // Calculate expected size of the tx buffer
  // ------- preamb + preamb + s_addr + d_addr + length + buffer + checksum
  size_t tx_buf_expected_size = 1 + 1 + 2 + 2 + 1 + pdu->body_len + 2;
  if (bufSize < tx_buf_expected_size)
  {
    return ERR_NOMEM;
  }

  // Start serializing the txBuf
  txBuf[0] = Msg_PDU_PREAMBLE0;
  txBuf[1] = Msg_PDU_PREAMBLE1;
  txBuf[2] = (uint8_t)(pdu->src_addr >> 8); // high-byte first
  txBuf[3] = pdu->src_addr & 0x00FF; // low-byte second
  txBuf[4] = (uint8_t)(pdu->dest_addr >> 8); // high-byte
  txBuf[5] = pdu->dest_addr & 0x00FF; // low-byte
  txBuf[6] = pdu->body_len;

  // Store next bytes accordingly if the buffer is not 0
  int next_byte = 7;
  for (size_t i = 0; i < pdu->body_len; i++)
  {
    txBuf[next_byte++] = pdu->body[i];
  }

  // Calculate and store checksum
  const uint16_t checksum = calculate_checksum(pdu->body, pdu->body_len);
  txBuf[next_byte++] = checksum >> 8;
  txBuf[next_byte] = checksum & 0x00FF;

  return 1;
}

static const uint16_t bigEndian2ByteToUint16(uint8_t high_byte, uint8_t low_byte)
{
  const uint16_t masked_low_byte = low_byte & 0xFFFF; // low B with high set to 0xFF
  uint16_t masked_high_byte = high_byte << 8; // put high B in high slot
  masked_high_byte |= 0x00FF; // turn low B to 0xFF
  return masked_high_byte & masked_low_byte;
}

/*
 * @see msg_pdu.h for description
 */
int receiveMessage(uint8_t const * const rxBuf, size_t const bufSize, Msg_PDU * const pdu)
{
  // If the received buffer is not at least 9 bytes long, throw error
  if (bufSize < 9)
  {
    return ERR_RCVD_BUF_TRUNCD;
  }

  // If pdu is unalloc'd, return
  if (NULL == pdu)
  {
    return ERR_NULL_PDU;
  }

  // Check preamble messages
  if (Msg_PDU_PREAMBLE0 != rxBuf[0])
  {
    return ERR_PREAMB0_MISMATCH;
  }
  else if (Msg_PDU_PREAMBLE1 != rxBuf[1])
  {
    return ERR_PREAMB1_MISMATCH;
  }

  // Check if source address is valid
  const uint16_t full_source_address = bigEndian2ByteToUint16(rxBuf[2], rxBuf[3]);
  if (get_my_addr() == full_source_address)
  {
    return ERR_WRONG_SRC;
  }
  // Set the source addr
  pdu->src_addr = full_source_address;

  // Check if destination address is valid
  const uint16_t full_dest_address = bigEndian2ByteToUint16(rxBuf[4], rxBuf[5]);
  if (get_my_addr != full_dest_address)
  {
    return ERR_WRONG_DST;
  }
  // Set dest addr
  pdu->dest_addr = full_dest_address;

  // Check body length to proceed
  const uint8_t body_length = rxBuf[6];
  if (0 != body_length)
  {
    memcpy(pdu->body, rxBuf[7], body_length);
  }

  // Verify checksum
  const uint16_t full_checksum = bigEndian2ByteToUint16(rxBuf[7+body_length], rxBuf[7+body_length+1]);
  if (calculate_checksum(pdu->body, body_length) != full_checksum)
  {
    return ERR_INVALID_CKSUM;
  }

  return 1;
}
