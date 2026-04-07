cmake_minimum_required(VERSION 3.31.7)

# [META] Project definition
project(back-prop LANGUAGES CXX)

# [CXX] Language standard configuration
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# [TOOL] Tooling and introspection
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# [SRCS] Explicit source file manifest
set(SOURCES
    src/main.cpp
    src/value.cpp
    src/util/graphing.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# [INCL] Header search paths
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# [FLAG] Core compiler options and warnings
target_compile_options(${PROJECT_NAME} PRIVATE
    -fcolor-diagnostics
    -pedantic -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wunused
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Debug>:-g -O0>
)

# [QUAL] Sanitizers and quality gates
option(USE_SANITIZERS "Enable ASan/UBSan" ON)
if(USE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE 
            -fsanitize=address,undefined -fno-omit-frame-pointer
        )
        target_link_options(${PROJECT_NAME} PRIVATE 
            -fsanitize=address,undefined
        )
    endif()
endif()

# [INST] Installation rules
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
