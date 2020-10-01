cmake_minimum_required(VERSION 3.17)
set(PROJECT_NAME P1)
project(P1)

set(CMAKE_CXX_STANDARD 11)

######################
##CLIENT APPLICATION##
######################

set(CLIENT_SRC_FILES
        main_client.cpp
        src/client.cpp
        )

set(CLIENT_INCLUDE_DIR
        include/client
        )

add_executable(${PROJECT_NAME}_client ${CLIENT_SRC_FILES})
target_include_directories(${PROJECT_NAME}_client PRIVATE ${CLIENT_INCLUDE_DIR})

######################
##SERVER APPLICATION##
######################

set(SERVER_SRC_FILES
        main_server.cpp
        src/server.cpp
        src/server.cpp
        src/quotedb.cpp
        )

set(SERVER_INCLUDE_DIR
        include/server
        )

add_executable(${PROJECT_NAME}_server ${SERVER_SRC_FILES})
target_include_directories(${PROJECT_NAME}_server PRIVATE ${SERVER_INCLUDE_DIR})

############
##DEMO APP##
############

file(GLOB_RECURSE CFILES "${CMAKE_SOURCE_DIR}/P1_docs/*.c")
set_source_files_properties(${CFILES} PROPERTIES LANUGAGE CXX)

add_executable(${PROJECT_NAME}_demo_server P1_docs/sampleP1/server.c)
add_executable(${PROJECT_NAME}_demo_client P1_docs/sampleP1/client.c)

###########
##TESTING##
###########

set(CATCH_INCLUDE_DIR include/external/catch2)

file(GLOB_RECURSE TEST_FILES "${CMAKE_SOURCE_DIR}/tests/*.cpp")
add_executable(${PROJECT_NAME}_tests ${TEST_FILES} ${SERVER_SRC_FILES} ${CLIENT_SRC_FILES})
target_include_directories(${PROJECT_NAME}_tests PRIVATE ${CATCH_INCLUDE_DIR} ${SERVER_INCLUDE_DIR} ${CLIENT_INCLUDE_DIR})