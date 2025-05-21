/*
**
**
**
**
*/
#pragma once
#include <cmath>
#include <algorithm>

/**
 * @brief RGB color representation for materials and rendering
 */
class Color {
    private:
        int m_r, m_g, m_b;
    public:
        /**
         * @brief Default constructor (black color)
         */
        Color();

        /**
         * @brief Constructor with RGB values
         */
        Color(int r, int g, int b);
        ~Color() = default;

        // Color component getters
        int getR() const;
        int getG() const;
        int getB() const;

        // Color component setters
        void setR(int r);
        void setG(int g);
        void setB(int b);

        // Color operations
        Color operator+(const Color& other) const;
        Color operator*(float factor) const;
        Color& operator+=(const Color& other);
        Color& operator*=(float factor);
        Color operator*(const Color& other) const;

        // Internal structure to represent color using floating-point components
        struct Float {
            float r, g, b;
            /**
             * @brief Default constructor (black color).
             */
            Float();
            /**
             * @brief Constructor with float RGB components.
             */
            Float(float r, float g, float b);
            /**
             * @brief Converts an integer-based Color (0–255) to a float-based one (0.0–1.0).
             */
            Float(const Color& c);
    
            // Float operations
            Float& operator+=(const Float& other);
            Float operator*(float factor) const;
                /**
                 * @brief Converts a float color (0.0–1.0) to an integer-based Color (0–255)
                 */
            Color toColor() const;
        };
};
