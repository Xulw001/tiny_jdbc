cmake_minimum_required (VERSION 3.0)
set(project_name tiny-jdbc)
project ("${project_name}")

include_directories(include tiny_reflect/include)
add_subdirectory(test)
add_subdirectory(drv)