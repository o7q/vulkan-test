mkdir build
cmake -S . -B build
cmake --build build -j8
glslc ./src/shader.vert -o ./src/vert.spv
glslc ./src/shader.frag -o ./src/frag.spv
cp ./build/vulkan_test ./vulkan_test
