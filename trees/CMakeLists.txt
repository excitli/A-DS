cmake_minimum_required(VERSION 3.10)
project(lab3)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указание компилятора (опционально, обычно определяется автоматически)
set(CMAKE_CXX_COMPILER g++)

# Создание исполняемого файла
add_executable(lab3 lab3.cpp)

# Опции компиляции для g++
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(lab3 PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()