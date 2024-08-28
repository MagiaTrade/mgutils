#!/bin/bash

#### There is problem in CLION running tests because of the error and critical logs I think, so it not show tests passing properly.
#### So the intent of this script is to build independently and run the tests to see if they are all passing.

# Save the current directory
current_dir=$(pwd)

# Define the build directory
build_dir="build_tests"

# Create the build directory if it doesn't exist
if [ ! -d "$build_dir" ]; then
  mkdir "$build_dir"
fi

# Navigate to the build directory
cd "$build_dir" || { echo "Failed to enter the directory $build_dir"; exit 1; }

# Run CMake with specified options
cmake -DCMAKE_BUILD_TYPE=Debug -DMGUTILS_BUILD_TESTS=ON -DMGUTILS_BUILD_EXAMPLES=OFF ..

# Build the project with make (or ninja if you prefer)
cmake --build .

# Check if the build was successful
if [ $? -ne 0 ]; then
  echo "Project build failed"
  cd "$current_dir"
  exit 1
fi

# Navigate to the docs directory inside the build directory
cd tests || { echo "Failed to enter the docs directory"; exit 1; }

# Execute the mgu_docs binary
./mgutils_tests

# Return to the original directory
cd "$current_dir" || { echo "Failed to return to the original directory"; exit 1; }

# Confirmation of successful execution
echo "Build and execution of mgu_tests completed successfully."
