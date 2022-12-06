#include "cpps/cst/expression.hpp"

#include "cpps/cst.hpp"
#include "cpps/source-location.hpp"

namespace CPPS::CST {

SourceLocation Expression::getLocation() const
{
    return assignment->getLocation();
}

} // namespace CPPS::CST
