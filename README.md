
my mac:

cmake -B build -S . -DCMAKE_PREFIX_PATH=/Users/sr/Qt/6.9.1/macos


on linux with ninja:

cmake -S . -B build -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/home/sr/Qt/6.9.0/gcc_64


