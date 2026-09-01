cmake -S ./extern/SDL3/ -B ./extern/SDL3/build/
cmake --build ./extern/SDL3/build/ --config Release
cmake --install ./extern/SDL3/build/ --prefix ./extern/install/ --config Release

cmake -S ./extern/bgfx.cmake/ -B ./extern/bgfx.cmake/build/
cmake --build ./extern/bgfx.cmake/build/ --config Release
cmake --install ./extern/bgfx.cmake/build/ --prefix ./extern/install/ --config Release