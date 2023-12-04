:: setup project, only needed once
:: cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja.exe -DCMAKE_TOOLCHAIN_FILE=~/Documents/vcpkg/scripts/buildsystems/vcpkg.cmake -G Ninja -S .. -B ../cmake-build-debug

:: build project
cmake --build ..\cmake-build-debug || (
    echo Build failed.
    pause
    exit /b
)

:: move exe to build dir
cmake --install ..\cmake-build-debug

:: run exe
..\cmake-build-debug\main