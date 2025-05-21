/*
**
**
**
**
*/

#include "RayTracer/Ray.hpp"

namespace RayTracer {

    Ray::~Ray()
    {}

    Ray::Ray(const Math::Point3D& origin, const Math::Vector3D& direction)
        : _origin(origin), _direction(direction.normalize())
    {}

// Math::Point3D Ray::pointAt(double t) const
// {
//     return Math::Point3D(
//         _origin._x + _direction._x * t,
//         _origin._y + _direction._y * t,
//         _origin._z + _direction._z * t
//     );
// }
//
}
