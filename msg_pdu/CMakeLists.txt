set(MSG_PDU_LIB_NAME msg_pdu)
set(MSG_PDU_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR})

# Produce msg_pdu.a (location is <repo>/build/msg_pdu/msg_pdu.a)
add_library(${MSG_PDU_LIB_NAME} STATIC
    msg_pdu.c
)
set_target_properties(${MSG_PDU_LIB_NAME} PROPERTIES
    PREFIX "" # Omit the "lib" prefix for the static lib
    # C_STANDARD 11 # Force -std=c11 for this target (C11 is enforced for the whole project already)
)

add_subdirectory(tests)
