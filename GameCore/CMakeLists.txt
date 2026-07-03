cmake_minimum_required(VERSION 3.14)
project(GameCore LANGUAGES CXX)

add_library(GameCore STATIC game_logic.cpp game_logic.h)

target_include_directories(GameCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})