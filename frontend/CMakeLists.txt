cmake_minimum_required(VERSION 3.10)
project(helen-frontend++)

# Set explicit paths to Clang++ and LLD
set(CMAKE_CXX_COMPILER /home/erick/llvm-project/build4/bin/clang++)
set(CMAKE_LINKER /home/erick/llvm-project/build4/bin/lld)
set(BISON /usr/bin/bison)
set(FLEX /usr/bin/flex)

message(STATUS "Using Clang++: ${CMAKE_CXX_COMPILER}")
message(STATUS "Using LLD: ${CMAKE_LINKER}")

# Set LLVM include directories (if needed - usually not necessary with explicit compiler/linker)
# If you get include errors, uncomment these and adjust paths if necessary.
#set(LLVM_INCLUDE_DIRS
#  "/home/user/llvmbuild/include" # Adjust if your includes are elsewhere
#)
#include_directories(${LLVM_INCLUDE_DIRS})


# Set compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funwind-tables -frtti")
add_definitions(-D_GNU_SOURCE -D_DEBUG -D_GLIBCXX_ASSERTIONS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)



# Define source files
set(SOURCE_FILES
  helen-frontend++.cc
  driver.cc
)

# Define files to be copied
set(FILES_TO_COPY
    "../parser.yy"        
    "../scanner.ll"
    "../AST.h"
    "../driver.cc"
    "../driver.hh"    
)

# Create a custom target for copying files
add_custom_target(copy_files
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${FILES_TO_COPY}
        ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying required files"
)

# Folder to save the generated files from bison and flex
set(GENERATED_SOURCES_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_SOURCES_DIR})

# Generate parser and scanner files from .yy and .ll files. Add dependency to copy_files.
add_custom_command(
  OUTPUT parser.cc scanner.cc
  DEPENDS copy_files
  COMMAND ${BISON} --html --graph -o parser.cc parser.yy
  COMMAND ${FLEX} -o scanner.cc scanner.ll
  VERBATIM
  USES_TERMINAL
)

# Add parser and scanner to the source files
list(APPEND SOURCE_FILES parser.cc scanner.cc)

# Add source files
add_executable(helen-frontend++ ${SOURCE_FILES})

# Link with LLVM libraries (if needed - usually not necessary with explicit compiler/linker)
# If you get linker errors, uncomment these and adjust paths if necessary.
#set(LLVM_LIBRARY_DIRS "/home/user/llvmbuild/lib") # Adjust if your libraries are elsewhere
#link_directories(${LLVM_LIBRARY_DIRS})
#target_link_libraries(helen-frontend++ -lLLVMSupport) # Or other necessary LLVM libs

# Ensure the executable depends on the file copying
add_dependencies(helen-frontend++ copy_files)

# Set the linker explicitly for the target (more robust)
set_target_properties(helen-frontend++ PROPERTIES LINKER_LANGUAGE CXX) # Important for LLD to be used correctly