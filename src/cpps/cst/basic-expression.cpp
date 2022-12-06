#include "cpps/cst/basic-expression.hpp"

#include "cpps/cst.hpp"
#include "cpps/source-location.hpp"

namespace CPPS::CST {

SourceLocation BasicExpression::getLocation() const
{
    const SourceLocation location = prefix->getLocation();

    if (location == InvalidSourceLocation)
    {
        return primary->getLocation();
    }

    return location;
}

} // namespace CPPS::CST
