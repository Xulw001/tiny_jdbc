cmake_minimum_required (VERSION 3.0)
set(project_name libdrv-sqlite)
project ("${project_name}")

include_directories(.)
link_libraries(sqlite3)
aux_source_directory(. DIR_SRC)
add_library(${project_name} SHARED ${DIR_SRC})
set_target_properties(${project_name} PROPERTIES PREFIX "")

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../../output" CACHE PATH "输出目录" FORCE)
install(TARGETS ${project_name} DESTINATION .)