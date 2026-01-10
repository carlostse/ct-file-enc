#-------------------------------------------------
#
# CT File Encrypt
#
# Author: Carlos Tse
# Email: carlos@aboutmy.info
# Date: Jul 06, 2025
#-------------------------------------------------

cmake_minimum_required(VERSION 3.31)
project(ct_file_enc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# require to set OPENSSL_ROOT_DIR to OpenSSL installation
find_package(OpenSSL REQUIRED)

# require to set Qt5_DIR to Qt5 installation
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)

add_executable(ct_file_enc
        byte_array.cpp
        clickable_label.cpp
        drop_area.cpp
        drop_window.cpp
        main.cpp
        master_key.cpp
        util.cpp
        main.cpp
        resource.rc
)

target_include_directories(ct_file_enc
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${OPENSSL_INCLUDE_DIR}
)

IF(WIN32)
    target_link_libraries(ct_file_enc PRIVATE userenv.lib)
ENDIF(WIN32)

target_link_libraries(ct_file_enc
        PRIVATE OpenSSL::SSL OpenSSL::Crypto
        PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets
)

set_target_properties(ct_file_enc PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
)
