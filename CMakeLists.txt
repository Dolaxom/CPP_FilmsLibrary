cmake_minimum_required(VERSION 3.16)

project(
  filmslibrary
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Установка Google Tests, если их нет
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Директория создания исполняемого файла
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Включение каталогов с заголовочными файлами Crow и PostgreSQL
include_directories("third-party/Crow/include")
include_directories("third-party/json_nlohmann")
include_directories(${PostgreSQL_INCLUDE_DIRS})

# Добавление директорий для коротких записей инклюдов
include_directories(src)

# Поиск Crow и PostgreSQL
find_package(Crow PATHS "${CMAKE_CURRENT_SOURCE_DIR}/third-party/Crow/cmake")
find_package(PostgreSQL REQUIRED)

add_executable(${PROJECT_NAME}
  src/main.cc
  src/server/endpoints/endpoints.cc
  src/server/endpoints/endpoints.h
  src/database/postgres-wrapper.cc
  src/database/postgres-wrapper.h
  src/database/redis-wrapper.cc
  src/database/redis-wrapper.h
  src/utils/utils.h
  src/utils/logger.h
  src/server/server.cc
  src/server/server.h
  src/tests/tests.cc
  src/tests/tests.h
)

if(APPLE)
  include(".local/macos.cmake")
endif()

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PostgreSQL_LIBRARIES} GTest::gtest_main -lhiredis)
