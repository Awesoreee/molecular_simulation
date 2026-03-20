cmake_minimum_required(VERSION 3.16) # Проверка версии CMake.
									# Если версия установленной программы
									# старее указаной, произойдёт аварийный выход.

#настраиваем стандарты языка
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(molecular_simulation)				# Название проекта

set(SOURCE_EXE src/main.cpp)			# Установка переменной со списком исходников

add_executable(main ${SOURCE_EXE})	# Создает исполняемый файл с именем main

add_subdirectory(SFML)				# Добавление подпроекта, указывается имя дирректории

target_link_libraries(main SFML)		# Линковка программы с библиотекой