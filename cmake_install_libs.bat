REM Debug

cmake -S ./extern/bgfx.cmake/ -B ./extern/bgfx.cmake/build/
cmake --build ./extern/bgfx.cmake/build/ --config Debug -j12
cmake --install ./extern/bgfx.cmake/build/ --prefix ./extern/install/Debug/ --config Debug

cmake -S ./extern/SDL3/ -B ./extern/SDL3/buildDebug/
cmake --build ./extern/SDL3/buildDebug/ --config Debug -j12
cmake --install ./extern/SDL3/buildDebug/ --prefix ./extern/install/Debug/ --config Debug

cmake -S ./extern/SDL_image/ -B ./extern/SDL_image/build/ -D CMAKE_INSTALL_PREFIX="./extern/install/Debug/cmake/" -D SDLIMAGE_AVIF=OFF
cmake --build ./extern/SDL_image/build/ --config Debug -j12
cmake --install ./extern/SDL_image/build/ --prefix ./extern/install/Debug/ --config Debug

cmake -S ./extern/SDL_ttf/ -B ./extern/SDL_ttf/build/ -DCMAKE_INSTALL_PREFIX=./extern/install/Debug/cmake/
cmake --build ./extern/SDL_ttf/build/ --config Debug -j12
cmake --install ./extern/SDL_ttf/build/ --prefix ./extern/install/Debug/ --config Debug

cmake -S ./extern/SDL_shadercross/ -B ./extern/SDL_shadercross/build/ -DCMAKE_INSTALL_PREFIX=./extern/install/Debug/cmake/ -D SDLSHADERCROSS_VENDORED=ON -D SDLSHADERCROSS_DXC=ON -D SDLSHADERCROSS_SHARED=ON -D SDLSHADERCROSS_INSTALL=ON
cmake --build ./extern/SDL_shadercross/build/ --config Debug -j12
cmake --install ./extern/SDL_shadercross/build/ --prefix ./extern/install/Debug/ --config Debug

cmake -S ./extern/entt/ -B ./extern/entt/build/ -D ENTT_INSTALL=ON
cmake --build ./extern/entt/build/ --config Debug -j12
cmake --install ./extern/entt/build/ --prefix ./extern/install/Debug/ --config Debug

cmake -S ./extern/glm/ -B ./extern/glm/build/
cmake --build ./extern/glm/build/ --config Debug -j12
cmake --install ./extern/glm/build/ --prefix ./extern/install/Debug/ --config Debug

cmake -S ./extern/json/ -B ./extern/json/build/ -D JSON_MultipleHeaders=OFF
cmake --build ./extern/json/build/ --config Debug -j12
cmake --install ./extern/json/build/ --prefix ./extern/install/Debug/ --config Debug

cmake -S ./extern/assimp/ -B ./extern/assimp/build/
cmake --build ./extern/assimp/build/ --config Debug -j12
cmake --install ./extern/assimp/build/ --prefix ./extern/install/Debug/ --config Debug

cmake -S ./extern/JoltPhysics/Build -B ./extern/JoltPhysics/bin/
cmake --build ./extern/JoltPhysics/bin/ --config Debug -j12
cmake --install ./extern/JoltPhysics/bin/ --prefix ./extern/install/Debug/ --config Debug

REM Release

cmake -S ./extern/bgfx.cmake/ -B ./extern/bgfx.cmake/build/
cmake --build ./extern/bgfx.cmake/build/ --config Release -j12
cmake --install ./extern/bgfx.cmake/build/ --prefix ./extern/install/ --config Release

cmake -S ./extern/SDL3/ -B ./extern/SDL3/build/
cmake --build ./extern/SDL3/build/ --config Release -j12
cmake --install ./extern/SDL3/build/ --prefix ./extern/install/ --config Release

cmake -S ./extern/SDL_image/ -B ./extern/SDL_image/build/ -D CMAKE_INSTALL_PREFIX="./extern/install/cmake/" -D SDLIMAGE_AVIF=OFF
cmake --build ./extern/SDL_image/build/ --config Release -j12
cmake --install ./extern/SDL_image/build/ --prefix ./extern/install/ --config Release

cmake -S ./extern/SDL_ttf/ -B ./extern/SDL_ttf/build/ -DCMAKE_INSTALL_PREFIX=./extern/install/cmake/
cmake --build ./extern/SDL_ttf/build/ --config Release -j12
cmake --install ./extern/SDL_ttf/build/ --prefix ./extern/install/ --config Release

cmake -S ./extern/SDL_shadercross/ -B ./extern/SDL_shadercross/build/ -DCMAKE_INSTALL_PREFIX=./extern/install/cmake/ -D SDLSHADERCROSS_VENDORED=ON -D SDLSHADERCROSS_DXC=ON -D SDLSHADERCROSS_SHARED=ON -D SDLSHADERCROSS_INSTALL=ON
cmake --build ./extern/SDL_shadercross/build/ --config Release -j12
cmake --install ./extern/SDL_shadercross/build/ --prefix ./extern/install/ --config Release

cmake -S ./extern/entt/ -B ./extern/entt/build/ -D ENTT_INSTALL=ON
cmake --build ./extern/entt/build/ --config Release -j12
cmake --install ./extern/entt/build/ --prefix ./extern/install/ --config Release

cmake -S ./extern/glm/ -B ./extern/glm/build/
cmake --build ./extern/glm/build/ --config Release -j12
cmake --install ./extern/glm/build/ --prefix ./extern/install/ --config Release

cmake -S ./extern/json/ -B ./extern/json/build/ -D JSON_MultipleHeaders=OFF
cmake --build ./extern/json/build/ --config Release -j12
cmake --install ./extern/json/build/ --prefix ./extern/install/ --config Release

cmake -S ./extern/assimp/ -B ./extern/assimp/build/
cmake --build ./extern/assimp/build/ --config Release -j12
cmake --install ./extern/assimp/build/ --prefix ./extern/install/ --config Release

cmake -S ./extern/JoltPhysics/Build -B ./extern/JoltPhysics/bin/
cmake --build ./extern/JoltPhysics/bin/ --config Release -j12
cmake --install ./extern/JoltPhysics/bin/ --prefix ./extern/install/ --config Release