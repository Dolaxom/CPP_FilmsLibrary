cmake_minimum_required(VERSION 3.16)

project(
  filmslibrary
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# Директория создания исполняемого файла
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Поиск Crow и PostgreSQL
find_package(Crow REQUIRED)
find_package(PostgreSQL REQUIRED)

# Включение каталогов с заголовочными файлами Crow и PostgreSQL
include_directories("third-party/Crow/include")
include_directories("third-party/json_nlohmann")
include_directories(${PostgreSQL_INCLUDE_DIRS})

# Добавление директорий для коротких записей инклюдов
include_directories(src)

add_executable(${PROJECT_NAME}
  src/main.cc
  src/server/endpoints.cc
  src/server/endpoints.h
  src/database/wrapper.cc
  src/database/wrapper.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE Crow::Crow ${PostgreSQL_LIBRARIES})
