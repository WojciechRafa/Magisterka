#include <cstdlib>

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
