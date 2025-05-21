/*
** PrimitiveConfig - Data structures for primitive configuration
*/

#pragma once
#include "Utils/Color.hpp"
#include "Math/Vector3D.hpp"
#include "Math/Point3D.hpp"
#include <string>
#include <variant>

/**
 * @brief Specific data for sphere primitives
 */
typedef struct SphereData_s {
    Math::Point3D c;   ///< Center point of the sphere
    double r;          ///< Radius of the sphere
    SphereData_s(Math::Point3D c_, double r_)
      : c(c_), r(r_) {}
} SphereData_t;

/**
 * @brief Specific data for triangle primitives
 */
typedef struct TriangleData_s {
    Math::Point3D a;   ///< First vertex
    Math::Point3D b;   ///< Second vertex
    Math::Point3D c;   ///< Third vertex
} TriangleData_t;

/**
 * @brief Specific data for rectangle primitives
 */
typedef struct RectangleData_s {
    Math::Point3D origin;     ///< Origin point of the rectangle
    Math::Vector3D bottom;    ///< Bottom side vector
    Math::Vector3D left;      ///< Left side vector
} RectangleData_t;

/**
 * @brief Specific data for plane primitives
 */
typedef struct PlaneData_s {
    Math::Point3D pos;       ///< A point on the plane
    Math::Vector3D norm;     ///< Normal vector to the plane
} PlaneData_t;

/**
 * @brief Specific data for cone primitives
 */
typedef struct ConeData_s {
    Math::Point3D apex;      ///< Apex (top) point of the cone
    Math::Vector3D axis;     ///< Direction vector from apex to base
    double radius;           ///< Radius of the base
    double height;           ///< Height from apex to base
} ConeData_t;

/**
 * @brief Specific data for cylinder primitives
 */
typedef struct CylinderData_s {
    Math::Point3D baseCenter;    ///< Center of the base circle
    Math::Vector3D axis;         ///< Direction vector from base to top
    double radius;               ///< Radius of the cylinder
    double height;               ///< Height of the cylinder
} CylinderData_t;

/**
 * @brief Variant type to hold primitive-specific data
 */
using SpecificData = std::variant<std::monostate, SphereData_t, TriangleData_t, RectangleData_t, PlaneData_t, ConeData_t, CylinderData_t>;

/**
 * @brief Configuration structure for primitive creation
 */
struct PrimitiveConfig {
    std::string type;    ///< Type identifier of the primitive
    Color color;         ///< Color of the primitive
    SpecificData data;   ///< Type-specific data for the primitive
};
