#pragma once
#include "Math/Vector3D.hpp"

namespace Core {
    class ITransformable {
        public:
            virtual ~ITransformable() = default;

            /**
             * @brief Moves the object by a specified offset
             * @param offset Vector representing the direction and distance to move
             */
            virtual void translate(const Math::Vector3D& offset) = 0;
    };
}
