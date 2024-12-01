# Menentukan minimal CMake version yang dibutuhkan
cmake_minimum_required(VERSION 3.10)

# Nama proyek
project(HandlerApp C)

# Tentukan standar bahasa C yang digunakan
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Tentukan file sumber proyek
set(SOURCES
    main.c
    sockutils/sockutils.c
)

# Tentukan file header (jika diperlukan untuk autocomplete atau dokumentasi)
include_directories(handler)

# Membuat executable dengan file sumber
add_executable(handler_app ${SOURCES})

# Jika Anda memiliki libraries eksternal yang perlu ditautkan, bisa menggunakan:
# target_link_libraries(handler_app <library_name>)
