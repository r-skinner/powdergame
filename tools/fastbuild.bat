cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=/tools/ninja.exe -DCMAKE_TOOLCHAIN_FILE=~/Documents/vcpkg/scripts/buildsystems/vcpkg.cmake -G Ninja -S .. -B ../cmake-build-debug

cmake --build ../cmake-build-debug || (
    echo Build failed.
    pause
    exit /b
)

cmake --install ../cmake-build-debug

pause