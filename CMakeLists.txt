cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME P2)
project(P2)

set(CMAKE_CXX_STANDARD 98)

######################
#####FIND PACKAGES####
######################

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

######################
##CLIENT APPLICATION##
######################

set(CLIENT_SRC_FILES
        src/client.cpp
        )

set(CLIENT_INCLUDE_DIR
        include/client
        )

add_executable(${PROJECT_NAME}_client main_client.cpp ${CLIENT_SRC_FILES})
target_include_directories(${PROJECT_NAME}_client PRIVATE ${CLIENT_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_client PRIVATE Threads::Threads)

######################
##SERVER APPLICATION##
######################

set(SERVER_SRC_FILES
        src/server.cpp
        src/helper.hpp
        src/quotedb.cpp
        )

set(SERVER_INCLUDE_DIR
        include/server
        )

add_executable(${PROJECT_NAME}_server main_server.cpp ${SERVER_SRC_FILES})
target_include_directories(${PROJECT_NAME}_server PRIVATE ${SERVER_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_server PRIVATE Threads::Threads)

############
##DEMO APP##
############

file(GLOB_RECURSE CFILES "${CMAKE_SOURCE_DIR}/P1_docs/*.cpp")
set_source_files_properties(${CFILES} PROPERTIES LANUGAGE CXX)

add_executable(${PROJECT_NAME}_demo_server P2_docs/sampleP2/server_demo.cpp)
add_executable(${PROJECT_NAME}_demo_client P2_docs/sampleP2/client_demo.cpp)

target_link_libraries(${PROJECT_NAME}_demo_server PRIVATE Threads::Threads)
target_link_libraries(${PROJECT_NAME}_demo_client PRIVATE Threads::Threads)

###########
##TESTING##
###########

set(CATCH_INCLUDE_DIR include/external/catch2)

file(GLOB_RECURSE TEST_FILES "${CMAKE_SOURCE_DIR}/tests/*.cpp")
add_executable(${PROJECT_NAME}_tests ${TEST_FILES} ${SERVER_SRC_FILES} ${CLIENT_SRC_FILES})
target_include_directories(${PROJECT_NAME}_tests PRIVATE ${CATCH_INCLUDE_DIR} ${SERVER_INCLUDE_DIR} ${CLIENT_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_tests PRIVATE Threads::Threads)