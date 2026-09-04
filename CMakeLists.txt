cmake_minimum_required(VERSION 3.31)

project(
  SolusEngine
  VERSION 1.0
  LANGUAGES CXX
)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

#############################

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/extern/fc_cache CACHE PATH "" FORCE)
include(FetchContent)

#############################

set(ENABLE_ALL_WARNINGS OFF)

set(GLM_ENABLE_CXX_20 ON)

set(SDLIMAGE_AVIF OFF)
set(SDLSHADERCROSS_VENDORED ON CACHE BOOL "" FORCE)
set(SDLSHADERCROSS_DXC OFF)
set(SDLSHADERCROSS_SHARED ON)

set(JSON_MultipleHeaders OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CPP_RTTI_ENABLED ON)

#############################

set(BUILD_SHARED_LIBS ON)
set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF)

# Include Jolt
FetchContent_Declare(
        JoltPhysics
        GIT_REPOSITORY "https://github.com/jrouwe/JoltPhysics"
        GIT_TAG "v5.6.0"
		SOURCE_SUBDIR "Build"
)
FetchContent_MakeAvailable(JoltPhysics)

set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
        GIT_TAG "release-3.4.14"
)
FetchContent_MakeAvailable(SDL3)

#set(BGFX_BUILD_TOOLS_SHADER OFF)
set(BGFX_BUILD_EXAMPLES OFF)

FetchContent_Declare(
        bgfx
        GIT_REPOSITORY "https://github.com/bkaradzic/bgfx.cmake.git"
        GIT_TAG "v1.156.9437-568"
)
FetchContent_MakeAvailable(bgfx)

FetchContent_Declare(
        assimp
        GIT_REPOSITORY "https://github.com/assimp/assimp.git"
        GIT_TAG "v6.0.5"
)
FetchContent_MakeAvailable(assimp)

FetchContent_Declare(
        glm
        GIT_REPOSITORY "https://github.com/g-truc/glm.git"
        GIT_TAG "1.0.3"
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
        entt
        GIT_REPOSITORY "https://github.com/skypjack/entt.git"
        GIT_TAG "v4.0.0"
)
FetchContent_MakeAvailable(entt)

set(nlohmann_json OFF)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY "https://github.com/nlohmann/json.git"
        GIT_TAG "v3.12.0"
)
FetchContent_MakeAvailable(nlohmann_json)

set(SKIP_PERFORMANCE_COMPARISON ON) # cereal requires boost if this is off
set(BUILD_SANDBOX OFF)

FetchContent_Declare(
        cereal
        GIT_REPOSITORY "https://github.com/USCiLab/cereal.git"
        GIT_TAG "v1.3.2"
)
FetchContent_MakeAvailable(cereal)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
        GIT_TAG "v1.92.9b-docking"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
        imguizmo
        GIT_REPOSITORY "https://github.com/CedricGuillemet/ImGuizmo.git"
        GIT_TAG "1.10"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        SOURCE_SUBDIR "INVALID" #use this so the CMakeLists.txt is not executed
)
FetchContent_MakeAvailable(imguizmo)

#set(SHADERC_SKIP_TESTS TRUE)
#set(SHADERC_SKIP_INSTALL TRUE)
#set(SHADERC_SKIP_EXAMPLES TRUE)
#set(SHADERC_SKIP_EXECUTABLES TRUE)
#set(SHADERC_SKIP_COPYRIGHT_CHECK TRUE)

#FetchContent_Declare(
#        libshaderc
#        GIT_REPOSITORY "https://github.com/google/shaderc.git"
#        GIT_TAG "v2026.3"
#)
#FetchContent_MakeAvailable(libshaderc)

#FetchContent_Declare(
#        spirv_tools
#        GIT_REPOSITORY "https://github.com/KhronosGroup/SPIRV-Tools.git"
#        GIT_TAG "v2026.3"
#)
#FetchContent_MakeAvailable(spirv_tools)

#############################

file(GLOB_RECURSE headers CONFIGURE_DEPENDS 
    ${CMAKE_CURRENT_SOURCE_DIR}/SEngine/include/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SEngine/include/**/*.hpp)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS 
    ${CMAKE_CURRENT_SOURCE_DIR}/SEngine/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SEngine/src/**/*.cpp)

# IMGUI

add_library(imgui INTERFACE)
target_include_directories(imgui INTERFACE ${imgui_SOURCE_DIR})

file(GLOB imgui_sources CONFIGURE_DEPENDS 
    ${imgui_SOURCE_DIR}/*.cpp)

list(APPEND sources ${imgui_sources})

add_library(imguizmo INTERFACE)
target_include_directories(imguizmo INTERFACE ${imguizmo_SOURCE_DIR}/src)

file(GLOB imguizmo_sources CONFIGURE_DEPENDS 
    ${imguizmo_SOURCE_DIR}/src/*.cpp)

list(APPEND sources ${imguizmo_sources})

# ~IMGUI

add_executable(${PROJECT_NAME} WIN32 ${headers} ${sources})

target_compile_options(${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

add_definitions(/FI "${CMAKE_CURRENT_SOURCE_DIR}/SEngine/include/SEnginePCH.hpp")
add_definitions(-DBX_CONFIG_DEBUG)

target_include_directories(${PROJECT_NAME} 
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/SEngine/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/extern/stb>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/extern/sdf>
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
target_link_libraries(${PROJECT_NAME} PRIVATE bgfx)
target_link_libraries(${PROJECT_NAME} PRIVATE bimg)
target_link_libraries(${PROJECT_NAME} PRIVATE bx)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE Jolt)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm-header-only)
target_link_libraries(${PROJECT_NAME} PRIVATE EnTT::EnTT)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE cereal::cereal)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE imguizmo)
#target_link_libraries(${PROJECT_NAME} PRIVATE shaderc_combined)

if (MSVC)
	# 64 bit architecture
	set(CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE "x64")

	# Set runtime library
	if (USE_STATIC_MSVC_RUNTIME_LIBRARY)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif()

	# Set general compiler flags
	target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus /Gm- /MP /nologo /diagnostics:classic /FC /fp:except- /Zc:inline /Zc:preprocessor>)

	# Enable warnings
	if (ENABLE_ALL_WARNINGS)
		target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/Wall /WX>)
	endif()
	
	# Optionally generate debug symbols
	if (GENERATE_DEBUG_SYMBOLS)
		target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/Zi>)
	endif()

	if (NOT CPP_RTTI_ENABLED)
		# Set compiler flag for disabling RTTI
		target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/GR->)
	else()
		# Set compiler flag for enabling RTTI
		target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/GR>)
	endif()

	if (NOT CPP_EXCEPTIONS_ENABLED)
		# Remove any existing compiler flag that enables exceptions
		#string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

		# Disable warning about STL and compiler-generated types using noexcept when exceptions are disabled
		target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/wd4577>)
	else()
		# Enable exceptions
		target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/EHsc>)
	endif()

	# Set compiler flags for various configurations
	if (OVERRIDE_CXX_FLAGS)
		#set(CMAKE_CXX_FLAGS_DEBUG "/GS /Od /Ob0 /RTC1")
		#set(CMAKE_CXX_FLAGS_RELEASE "/GS- /Gy /O2 /Oi /Ot")
	endif()

	# Set linker flags
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		if (CROSS_PLATFORM_DETERMINISTIC)
			target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/fp:precise>)
		else()
			target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/fp:fast>) # Clang doesn't use fast math because it cannot be turned off inside a single compilation unit
		endif()
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /showFilenames")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments") # Clang emits warnings about unused arguments such as /MP and /GL
	endif()
else()
	# Enable warnings
	if (ENABLE_ALL_WARNINGS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
	endif()

	# Optionally generate debug symbols
	if (GENERATE_DEBUG_SYMBOLS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
	endif()

	if (NOT CPP_RTTI_ENABLED)
		# Set compiler flag for disabling RTTI
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
	else()
		# Set compiler flag for enabling RTTI
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti")
	endif()

	if (NOT CPP_EXCEPTIONS_ENABLED)
		# Set compiler flag for disabling exception-handling
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
	else()
		# Set compiler flag for enabling exception-handling
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
	endif()

	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		# Also disable -Wstringop-overflow or it will generate false positives that can't be disabled from code when link-time optimizations are enabled
		# Also turn off automatic fused multiply add contractions, there doesn't seem to be a way to do this selectively through the macro JPH_PRECISE_MATH_OFF
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-stringop-overflow -ffp-contract=off")
	else()
		# Do not use -ffast-math since it cannot be turned off in a single compilation unit under clang, see Core.h
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffp-model=precise")

		# On clang 14 and later we can turn off float contraction through a pragma, older versions and deterministic versions need it off always, see Core.h
		if (CMAKE_CXX_COMPILER_VERSION LESS 14 OR CROSS_PLATFORM_DETERMINISTIC)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffp-contract=off")
		endif()
	endif()

	# See https://github.com/jrouwe/JoltPhysics/issues/922. When compiling with DOUBLE_PRECISION=YES and CMAKE_OSX_DEPLOYMENT_TARGET=10.12 clang triggers a warning that we silence here.
	if ("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin" AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -faligned-allocation")
	endif()

	# Set compiler flags for various configurations
	if (OVERRIDE_CXX_FLAGS)
		set(CMAKE_CXX_FLAGS_DEBUG "")
		set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	endif()
	set(CMAKE_CXX_FLAGS_DISTRIBUTION "${CMAKE_CXX_FLAGS_RELEASE}")
endif()

#############################
#TODO move to util cmake file

add_custom_target(CopyEditorAssets
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/SEngine/assets/"
        "$<TARGET_FILE_DIR:SolusEngine>/assets/"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${bgfx_SOURCE_DIR}/bgfx/src/bgfx_shader.sh"
        "$<TARGET_FILE_DIR:SolusEngine>/assets/shaders/bgfx_shader.sh"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${bgfx_SOURCE_DIR}/bgfx/src/bgfx_compute.sh"
        "$<TARGET_FILE_DIR:SolusEngine>/assets/shaders/bgfx_compute.sh"

    COMMENT "Copying library binaries and editor assets"
)
add_dependencies(${PROJECT_NAME} CopyEditorAssets)
#error copy needs to depends on shader compilation

set(PRJ_SHADERS_COMPILER "extern/install/bin/shaderc.exe")
set(shader_src)
macro(compileShaders SRCS)
    foreach(FILE ${SRCS})
        get_filename_component(ARRAY_NAME "${FILE}" NAME_WE)
        
        if ("${FILE}" MATCHES ".*\\.sc")
            continue()
        endif()
        
        if ("${FILE}" MATCHES ".*\\.fs")
           set(SHADER_TYPE "fragment")
           set(ARRAY_NAME "${ARRAY_NAME}_fs")
        elseif ("${FILE}" MATCHES ".*\\.vs")
            set(SHADER_TYPE "vertex")
            set(ARRAY_NAME "${ARRAY_NAME}_vs")
        elseif ("${FILE}" MATCHES ".*\\.cs")
            set(SHADER_TYPE "compute")
            set(ARRAY_NAME "${ARRAY_NAME}_cs")
        endif()
        
        list(APPEND gen_src ${FILE}.h)
        #add_custom_command(
        #    OUTPUT ${FILE}.h
        #    COMMAND ${PRJ_SHADERS_COMPILER}
        #    -i extern/install/include/bgfx/ --type ${SHADER_TYPE} --platform "windows" -p spirv
        #    -f ${FILE} -o ${FILE}.bin# --bin2c ${ARRAY_NAME}
        #    DEPENDS ${FILE} ${PRJ_SHADERS_COMPILER}
        #    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        #)

        add_custom_target("${ARRAY_NAME}_sc"
        COMMAND ${PRJ_SHADERS_COMPILER}
            -i extern/install/include/bgfx/ --type ${SHADER_TYPE} --platform "windows" -p spirv
            -f ${FILE} -o ${FILE}.bin# --bin2c ${ARRAY_NAME}
            --varyingdef ${PROJECT_SOURCE_DIR}/SEditor/assets/shaders/varying.def.sc
            #DEPENDS ${FILE} ${PRJ_SHADERS_COMPILER}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        add_dependencies(SEditor "${ARRAY_NAME}_sc")

        #add_custom_command(
        #    TARGET SEditor
        #    POST_BUILD
        #    COMMAND ${PRJ_SHADERS_COMPILER}
        #    -i extern/install/include/bgfx/ --type ${SHADER_TYPE} --platform "windows" -p spirv
        #    -f ${FILE} -o ${FILE}.bin# --bin2c ${ARRAY_NAME}
        #    --varyingdef ${PROJECT_SOURCE_DIR}/SEditor/assets/shaders/varying.def.sc
        #    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        #)
    endforeach()

endmacro()
    

file(GLOB ALL_SHADERS
    ${PROJECT_SOURCE_DIR}/SEditor/assets/shaders/*.vs
    ${PROJECT_SOURCE_DIR}/SEditor/assets/shaders/*.fs
    ${PROJECT_SOURCE_DIR}/SEditor/assets/shaders/*.cs
    ${PROJECT_SOURCE_DIR}/SEditor/assets/shaders/*.sc
)
#compileShaders("${ALL_SHADERS}")