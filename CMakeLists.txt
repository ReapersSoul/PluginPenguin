project(PluginPenguin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set language to C++
enable_language(CXX)

#glob all source files ignore the vcpkg_installed directory
file(GLOB_RECURSE HEADERS "*.hpp")
file(GLOB_RECURSE SOURCES "*.cpp")
list(FILTER HEADERS EXCLUDE REGEX "vcpkg_installed")
list(FILTER SOURCES EXCLUDE REGEX "vcpkg_installed")

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

#set include directory
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Include/${PROJECT_NAME})
file(MAKE_DIRECTORY ${INCLUDE_DIR})

#copy headers to include directory making sure to preserve directory structure
foreach(header ${HEADERS})
  file(RELATIVE_PATH relPath ${CMAKE_CURRENT_SOURCE_DIR} ${header})
  get_filename_component(dir ${relPath} DIRECTORY)
  file(MAKE_DIRECTORY ${INCLUDE_DIR}/${dir})
  configure_file(${header} ${INCLUDE_DIR}/${relPath} COPYONLY)
endforeach()

#library output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${PROJECT_NAME}/${CMAKE_BUILD_TYPE}
)