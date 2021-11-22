# FORT Robotics Coding Evaluation - Arjan's Solution

## Dependencies

For building this project, cmake is required. There are many ways to install cmake. On Debian/Ubuntu systems, execute: `sudo snap install cmake --classic`.

## Building

Execute ./build.sh. It should build:

1. msg_pdu.a built with standard C11 enforced (main deliverable for this evaluation)
2. Tests that exercise the main 2 functions inside msg_pdu.a (GTest will auto-clone for this, internet connection required)

## Notes

The Makefile requested in the prompt of this evaluation is auto-generated by CMake and is located here: [this_repository]/build/Makefile
