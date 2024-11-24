cmake_minimum_required(VERSION 3.17)
project(Magisterka)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS})

add_executable(Magisterka
        main.cpp
        Network/sended_struct.hpp
        Network/sended_struct.cpp
        main_functions.hpp
        Network/Broadcast_Connector.cpp
        Network/Broadcast_Connector.hpp
        Network/Permanent_Connector.cpp
        Network/Permanent_Connector.hpp
        Network/Image_Receiver.cpp
        Network/Image_Receiver.hpp
        GUI/Buttons_Field.cpp
        GUI/Buttons_Field.hpp
        GUI/Graphic_Warehouse.cpp
        GUI/Graphic_Warehouse.hpp
        GUI/Button.cpp
        GUI/Button.hpp
        System.cpp
        System.hpp
        Time_Object.cpp
        Time_Object.hpp
        Connection/Connection_Frontend.cpp
        Connection/Connection_Frontend.hpp
        Connection/Connection_Backend.cpp
        Connection/Connection_Backend.hpp
        Connection/Connection.cpp
        Connection/Connection.hpp
        GUI/Graphic_Manager.cpp
        GUI/Graphic_Manager.hpp
        GUI/Custom_Data_IO_Window.cpp
        GUI/Custom_Data_IO_Window.hpp
        Network/Custom_Data_IO.cpp
        Network/Custom_Data_IO.hpp
        GUI/Small_window.cpp
        GUI/Small_window.hpp
        Image_Analysis/Image_source.cpp
        Image_Analysis/Image_source.hpp
        Image_Analysis/Binarization.cpp
        Image_Analysis/Binarization.hpp
        Image_Analysis/Projection_image_calculator.cpp
        Image_Analysis/Projection_image_calculator.hpp
        Network/Rays_receiver.cpp
        Network/Parameters_receiver.hpp
        Image_Analysis/Rays_intersection_analyzer.cpp
        Image_Analysis/Rays_intersection_analyzer.hpp
        Image_Analysis/Frame_parameters.hpp
        Rays_source.hpp
        Configs.hpp
        Objects_tracker.cpp
        Objects_tracker.hpp
        Rays_source.cpp
        GUI/Detected_objects_displayer.cpp
        GUI/Detected_objects_display.hpp
)

target_link_libraries(Magisterka sfml-network sfml-system sfml-window sfml-graphics ${OpenCV_LIBS})