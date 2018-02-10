cmake_minimum_required(VERSION 2.8)

if (UNIX AND NOT APPLE)
  execute_process(COMMAND ${CMAKE_C_COMPILER} -fuse-ld=gold -Wl,--version ERROR_QUIET OUTPUT_VARIABLE ld_version)
  if ("${ld_version}" MATCHES "GNU gold")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold -Wl,--disable-new-dtags")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=gold -Wl,--disable-new-dtags")
  endif()
endif()

project("infinirender" C)

set(CMAKE_BUILD_TYPE Debug)
add_definitions ("-Wall")

set (INSTEON_LIBS
)

# Our files:

set (INSTEON_CORE
# Core Stuff
src/msg.c
src/hashmap.c
)

#
# Build the executable
set (CONSOLE_EXEC src/main.c)
add_executable(console ${CONSOLE_EXEC} ${INSTEON_CORE})
target_link_libraries(console ${INSTEON_LIBS})
