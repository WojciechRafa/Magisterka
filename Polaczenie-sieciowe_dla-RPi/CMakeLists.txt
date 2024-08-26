cmake_minimum_required(VERSION 3.17)
project(Magisterka)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS})

add_executable(Magisterka
        main.cpp
        sended_struct.hpp
        sended_struct.cpp
        main_functions.hpp
        Time.hpp
        Broadcast_Connector.cpp
        Broadcast_Connector.hpp
        Pernament_Connector.cpp
        Pernament_Connector.hpp
        Window.cpp Window.hpp
        Image_Receiver.cpp
        Image_Receiver.hpp
        Buttons_Field.cpp
        Buttons_Field.hpp
        Graphic_Warehouse.cpp
        Graphic_Warehouse.hpp
        Button.cpp
        Button.hpp
        System.cpp
        System.hpp
        Time_Object.cpp
        Time_Object.hpp
        Connection_Frontend.cpp
        Connection_Frontend.hpp
        Connection_Backend.cpp
        Connection_Backend.hpp
        Connection.cpp
        Connection.hpp
        Graphic_Manager.cpp
        Graphic_Manager.hpp
        Custom_Data_IO_Window.cpp
        Custom_Data_IO_Window.hpp
        Custom_Data_IO.cpp
        Custom_Data_IO.hpp
        Small_window.cpp
        Small_window.hpp
        Image_source_typ.cpp
        Image_source_typ.hpp
        Binarization.cpp
        Binarization.hpp
        Projection_image_calculator.cpp
        Projection_image_calculator.hpp
        Rays_receiver.cpp
        Rays_receiver.hpp
)

target_link_libraries(Magisterka sfml-network sfml-system sfml-window sfml-graphics ${OpenCV_LIBS})