/*
**
**
**
**
*/



#pragma once
#include "Math/Point3D.hpp"
#include "Math/Vector3D.hpp"
#include "Utils/Color.hpp"

struct HitInfo {
    double t;     // distance le long du rayon
    Math::Point3D p;     // point d’impact
    Math::Vector3D n;    // normale (unitaire)
    const Color* color; // couleur de la primitive touchée
};
