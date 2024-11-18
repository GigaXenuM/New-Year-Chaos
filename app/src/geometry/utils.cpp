#include "utils.h"

#include <cmath>

namespace Geometry
{
double distance(PointF p1, PointF p2)
{
    return std::sqrt(std::pow(p1.x() - p2.x(), 2) + std::pow(p1.y() - p2.y(), 2));
}

} // namespace Geometry
