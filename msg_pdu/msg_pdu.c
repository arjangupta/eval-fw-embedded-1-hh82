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

/*
 * @see msg_pdu.h for description
 */
int receiveMessage(uint8_t const * const rxBuf, size_t const bufSize, Msg_PDU * const pdu)
{
  return 0;
}
