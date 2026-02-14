cmake_minimum_required (VERSION 3.0)
set(project_name drv-sample)
project ("${project_name}")

include_directories(. ../include ../tiny_reflect/include)
aux_source_directory(. DIR_SRC)
add_executable(${project_name} ${DIR_SRC})

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../output" CACHE PATH "输出目录" FORCE)
install(TARGETS ${project_name} DESTINATION .)
