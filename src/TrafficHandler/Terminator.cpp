#include "Terminator.h"

#include <iostream>

bool Terminator::handlerRequest(std::string& request)
{
    std::cout << request << std::endl;

    return true;
}

void Terminator::setSuccessor(Handler* /*successor*/)
{
}

Handler* Terminator::getNextSuccessor()
{
    return NULL;
}

bool Terminator::decode(std::string& bufer)
{
    return true;
}
