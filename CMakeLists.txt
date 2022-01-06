cmake_minimum_required(VERSION 3.2)

project(SemServerAlgPol C)

set(CMAKE_C_STANDARD 99)

add_executable(SemServerAlgPol main.c polynom.c polynom.h prvok.c prvok.h zlomok.c zlomok.h)
add_executable(SemKlient klient.c)
add_executable(SemServer server.c polynom.c polynom.h prvok.c prvok.h zlomok.c zlomok.h)
target_link_libraries(SemServer pthread)