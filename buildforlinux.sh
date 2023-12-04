# Create and enter the build directory
mkdir build
cd build

# Run CMake with the Linux toolchain and static triplet
cmake -DCMAKE_TOOLCHAIN_FILE=~/Documents/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-linux-static ..

# Build the project
cmake --build . --config Release || {
    echo "Build failed."
    exit 1
}

# Copy the built binary to the parent directory
cp -Y ./Release/main ..

cd ..

# Linux does not have a direct equivalent to ResourceHacker.exe for adding icons.
# You might need to embed the icon directly in your source code or use a different method.
# For example, if you're using a GUI toolkit like Qt or GTK, they have their own methods for setting application icons.

# Rename the binary file
mv main "Jackson's Powder Game"

# Pause (equivalent in bash)
read -p "Press enter to continue"