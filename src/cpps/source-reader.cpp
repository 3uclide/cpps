#include "cpps/source-reader.hpp"

#include <iostream>

namespace CPPS {

bool SourceReader::doSomething(bool isTrue)
{
    if (isTrue)
    {
        std::cout << "is true\n";
    }

    return isTrue;
}

}
