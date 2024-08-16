
#include <iostream>
#include <cstdlib>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <opencv2/opencv.hpp>

#include "sended_struct.hpp"
#include "main_functions.hpp"

#include "Time.hpp"

#include "Broadcast_Connector.hpp"
#include "Pernament_Connector.hpp"
#include "Image_Receiver.hpp"

#include "System.hpp"


int main() {
    auto system = System();

    while (true){
        bool is_end = system.update();

        if(is_end)
            break;
    }

    return EXIT_SUCCESS;
}
