cmake_minimum_required(VERSION 3.10)

project(TerminalTriviaGame VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build options
option(BUILD_TESTS "Build test suite" ON)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")

# Source files
set(SOURCES
    src/main.c
    src/game.c
    src/questions.c
    src/utils.c
    src/timer.c
)

# Header files
set(HEADERS
    src/game.h
    src/questions.h
    src/utils.h
    src/timer.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link pthread for timer functionality
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Install rules
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES data/questions.json DESTINATION share/${PROJECT_NAME})

# Testing
if(BUILD_TESTS)
    enable_testing()
    
    # Add test executable
    add_executable(test_${PROJECT_NAME}
        tests/test_main.c
        tests/test_utils.c
        tests/test_questions.c
        src/utils.c
        src/questions.c
    )
    
    target_include_directories(test_${PROJECT_NAME} PRIVATE src tests)
    target_link_libraries(test_${PROJECT_NAME} PRIVATE Threads::Threads)
    
    # Add tests
    add_test(NAME TestUtils COMMAND test_${PROJECT_NAME} utils)
    add_test(NAME TestQuestions COMMAND test_${PROJECT_NAME} questions)
    add_test(NAME TestAll COMMAND test_${PROJECT_NAME} all)
endif()

# Print configuration summary
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C Standard: ${CMAKE_C_STANDARD}")
message(STATUS "Build Tests: ${BUILD_TESTS}")

