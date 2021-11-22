#!/bin/sh

run_tests=0

# Process arguments
if [ ! -z $1 ]; then
    if { [ "--full-build" = $1 ] || [ "--full-rebuild" = $1 ]; }; then
        rm -rf build/
    elif [ "--run-tests" = $1 ]; then
        run_tests=1
    fi
fi

if { [ ! -d "build" ]; }; then
    # The build/ dir doesn't exist. Generate all build files.
    echo "---------------- Generating x86_64 build files ----------------"
    echo ""
    cmake -H. -Bbuild/
fi

echo ""
echo "---------------- Commencing build ----------------"
echo ""
cd build/
make -j $(nproc) #VERBOSE=1 #<------- comment in VERBOSE=1 to see flags
build_status=$?

if { [ "$run_tests" = "1" ]; }; then
    ctest --output-on-failure
fi

cd ../../

if { [ $build_status != 0 ]; }; then
    exit 101
else
    exit 0
fi
