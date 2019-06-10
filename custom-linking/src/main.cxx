#include <iostream>
#include "module.h"

int main()
{
    for (auto entry = customlink::moduleMap; entry != &customlink::moduleMapEnd; ++entry)
    {
        std::cout <<"Found module for id " <<entry->id <<", calling it...\n    ";
        entry->function();
    }
    return 0;
}
