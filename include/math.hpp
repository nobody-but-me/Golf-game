
#pragma once
#ifndef MATH_H
#define MATH_H

namespace Math {
    inline float lerp(float a, float b, float y);
}

#endif//MATH_H
#ifdef MATH_IMPLEMENTATION

#include <iostream>

namespace Math {
    inline float lerp(float a, float b, float t) {
	return a * (1.0 - t) + (b * t);
    }
}

#endif//MATH_IMPLEMENTATION
