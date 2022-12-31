cmake_minimum_required(VERSION 3.10)

project(incbin LANGUAGES C CXX)

add_library(${PROJECT_NAME} INTERFACE
	incbin.h
)

add_executable("${PROJECT_NAME}-exe"
	incbin.c
)

target_link_libraries("${PROJECT_NAME}-exe" PRIVATE
	incbin
)

target_include_directories(${PROJECT_NAME} INTERFACE ".")
target_include_directories("${PROJECT_NAME}-exe" PRIVATE ".")
