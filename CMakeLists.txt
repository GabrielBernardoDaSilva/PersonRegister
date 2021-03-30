cmake_minimum_required(VERSION 3.17)
project(PersonRegister)

set(CMAKE_CXX_STANDARD 17)

find_package(cpprestsdk REQUIRED)
find_package(nlohmann_json 3.7.0 REQUIRED)

add_executable(PersonRegister main.cpp Headers/Database.hpp Headers/Person.hpp src/DbPerson.cpp)

target_link_libraries(${PROJECT_NAME} cpprestsdk::cpprest)
target_link_libraries(${PROJECT_NAME} sqlite3)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
