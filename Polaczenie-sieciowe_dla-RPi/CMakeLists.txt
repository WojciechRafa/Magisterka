cmake_minimum_required(VERSION 3.16)
project(Magisterka)

set(OpenCV_DIR   "/home/wr/Biblioteki/OpenCV/opencv-4.x/cmake")

set(CMAKE_CXX_STANDARD 20)


find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS})

add_executable(Magisterka main.cpp
	main_functions.hpp
        Connection/Sended_struct.hpp Connection/Sended_struct.cpp
        Connection/Broadcast_connector.hpp Connection/Broadcast_connector.cpp
        Connection/Pernament_Connector.hpp Connection/Pernament_Connector.cpp
        Connection/Image_Sender.hpp Connection/Image_Sender.cpp
	Time.hpp
        Image_Analysis/Camera.hpp Image_Analysis/Camera.cpp
	System.cpp System.hpp
	Time_Object.cpp Time_Object.hpp
        Connection/Custom_Data_IO.cpp Connection/Custom_Data_IO.hpp
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
        Image_Analysis/Projection_image_calculate.cpp
        Image_Analysis/Projection_image_calculate.hpp
        Connection/Parameter_sender.cpp
        Connection/Parameter_sender.hpp
        Configs.hpp
)
        
target_link_libraries(Magisterka sfml-network sfml-system sfml-window sfml-graphics ${OpenCV_LIBS}
)
#target_compile_options(Magisterka PRIVATE -Wall)
