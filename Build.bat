conan install . --output-folder=Build --build=missing
pushd Build
cmake .. -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
cmake --build . --config Release
popd
