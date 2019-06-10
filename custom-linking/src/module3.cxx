#include <iostream>
#include "module.h"

CUSTOMLINK_MODULE(42)
{
    std::cout <<"Hello from module3.cxx" <<std::endl;
}
