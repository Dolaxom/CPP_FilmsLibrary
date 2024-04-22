Для пользователей MacOS необходимо добавить в этой директории файл macos.cmake с таким содержимым (как пример) для подключения Postgres:


include_directories(~/.brew/Cellar/postgresql@14/14.11_1/include/postgresql@14)


link_directories(~/.brew/Cellar/postgresql@14/14.11_1/lib/postgresql@14/) 


---

В файле database.conf необходимо прописать параметры для подключения к бд PostgreSQL