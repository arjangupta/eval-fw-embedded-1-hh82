#ifndef _MSG_PDU_H_
#define _MSG_PDU_H_

#include <stdint.h>

#define MAX_BODY_LEN 255

#define ERR_NOMEM (-1)
#define ERR_INVALID_CKSUM (-2)
#define ERR_WRONG_DST (-3)

/* 
 * You are writing a portion of an implementation of a device-to-device serial
 * line protocol. Messages conforming to the protcol follow the structure shown
 * in Table 1.
 *
 * Table 1 - Protocol Data Unit:
 *
 * +-----------------------+------------+------------+-----------------------+-----------------------+---------------------+------------------------+---------------------------------+
 * | Field Name:           | Preamble 0 | Preamble 1 | Source                | Destination           | Body Length         | Body                   | Checksum                        |
 * |                       |            |            | Address               | Address               |                     |                        |                                 |
 * +-----------------------+------------+------------+-----------------------+-----------------------+---------------------+------------------------+---------------------------------+
 * | Field Length (bytes): | 1          | 1          | 2                     | 2                     | 1                   | <Value of Body Length> | 2                               |
 * +-----------------------+------------+------------+-----------------------+-----------------------+---------------------+------------------------+---------------------------------+
 * | Value/Description:    | 0xAA       | 0xAA       | Transmitted in        | Transmitted in        | Indicates Length of | <Application Data>     | Calculated over fields          |
 * |                       |            |            | Big Endian Byte-Order | Big Endian Byte-Order | Body Field          | (Bytes may take any    | (inclusive): Preamble0 --> Body |
 * |                       |            |            |                       |                       |                     |  value in [0,255])     | Transmitted in Big Endian       |
 * |                       |            |            |                       |                       |                     |                        | Byte-Order                      |
 * +-----------------------+------------+------------+-----------------------+-----------------------+---------------------+------------------------+---------------------------------+
 *
 */

/* 
 * The Msg_PDU structure holds an in-memory representation of a protocol message
 * for on-device processing. 
 */
typedef struct Msg_PDU {
  uint16_t dest_addr;
  uint16_t src_addr;
  uint8_t body_len;
  uint8_t body[MAX_BODY_LEN];
} Msg_PDU;

/* 
 * @param buffer[in] Data to calculate the checksum of
 * @param buffer_len[in] The length of the data contained in <buffer>
 *
 * @return The calculated checksum
 */
// Stub function. Do not implement
inline uint16_t calculate_checksum(uint8_t const *const buffer, uint16_t buffer_len){
  return 0x2827;
}

/* 
 * @return The local device address
 */
// Stub function. Do not implement
inline uint8_t get_my_addr(){
    return 0x19;
}

/*
 * Prepares a PDU for transmssion out of the device's communications port.
 *
 * @param pdu[in] The in-memory representation of the data to send. This
 *                structure has buffer has been populated by the calling
 *                function.
 *
 * @param txBuf[in/out] A serialization buffer that will be passed to the
 *                      communications infrastructure for transmission. This
 *                      buffer should be populated with the full on-the-wire
 *                      representation of the PDU as shown in Table 1 based on
 *                      the data in <pdu>.
 *
 * @param bufSize[in] The size of the buffer pointed to by <txBuf>
 *
 * @return 1 on success, ERRNO for a known error, or 0 for an unknown error
 */
//TODO - Implement this function
int prepareMessage(Msg_PDU const * const pdu, uint8_t* const txBuf, size_t const bufSize);

/*
 * Process a received buffer of data for usage by the rest of the application
 *
 * @param rxBuf[in] The raw buffer content as received by the communications
 *                  subsystem. <rxBuf> is gauranteed by the receiving logic
 *                  to start on a message boundary (i.e. <0xAA><0xAA>), and to
 *                  contain exactly one full message.
 *
 * @param bufSize[in] Size of <rxBuf>
 *
 * @param pdu[in/out] Pointer to an already-allocated buffer where the internal
 *                    representation should be stored.
 *
 * @return 1 on success, ERRNO for a known error, or 0 for an unknown error
 */
//TODO - Implement this function
int receiveMessage(uint8_t const * const rxBuf, size_t const bufSize, Msg_PDU * const pdu);

#endif // _MSG_PDU_H_
