cmake_minimum_required(VERSION 3.16)
project(Magisterka)

set(OpenCV_DIR   "/home/wr/Biblioteki/OpenCV/opencv-4.x/cmake")

set(CMAKE_CXX_STANDARD 20)


find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS})

add_executable(Magisterka main.cpp
	main_functions.hpp
        Network/Sended_struct.hpp Network/Sended_struct.cpp
        Network/Broadcast_connector.hpp Network/Broadcast_connector.cpp
        Network/Pernament_Connector.hpp Network/Pernament_Connector.cpp
        Network/Image_Sender.hpp Network/Image_Sender.cpp
	Time.hpp
        Image_Analysis/Camera.hpp Image_Analysis/Camera.cpp
	System.cpp System.hpp
	Time_Object.cpp Time_Object.hpp
        Network/Custom_Data_IO.cpp Network/Custom_Data_IO.hpp
        GUI/Window.cpp
        GUI/Window.hpp
        GUI/Graphic_Warehouse.cpp
        GUI/Graphic_Warehouse.hpp
        GUI/Buttons_Field.hpp
        GUI/Buttons_Field.cpp
        GUI/Graphic_Manager.hpp
        GUI/Graphic_Manager.cpp
        GUI/Button.hpp
        GUI/Button.cpp
        GUI/Small_window.cpp
        GUI/Small_window.hpp
		Image_source.cpp
		Image_source.hpp
        Image_Analysis/Binarization.cpp
        Image_Analysis/Binarization.hpp
        Image_Analysis/Projection_image_calculator.cpp
        Image_Analysis/Projection_image_calculator.hpp
        Network/Parameter_sender.cpp
        Network/Parameter_sender.hpp
        Configs.hpp
)
        
target_link_libraries(Magisterka sfml-network sfml-system sfml-window sfml-graphics ${OpenCV_LIBS}
)
#target_compile_options(Magisterka PRIVATE -Wall)
