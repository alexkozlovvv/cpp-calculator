cmake_minimum_required(VERSION 3.11)

project(Calculator CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(calculator src/main.cpp 
src/calculator.cpp src/parser.cpp 
src/ast.cpp src/lexer.cpp src/token.cpp)

if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
else()
    set(SYSTEM_LIBS)
endif()

target_link_libraries(calculator ${SYSTEM_LIBS}) 

target_include_directories(calculator PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
