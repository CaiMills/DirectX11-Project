#pragma once
#include "Structures.h"

/// <summary>
/// This is just the base to include all my maths calculation rather than needing to type repeated code, some of these functions are available within different classes, but I wanted a consistant one.
/// </summary>

class Maths
{
public:
	float Magnitude(Vector3 a);
	Vector3 Normalise(Vector3 a);
	float Dot(Vector3 a, Vector3 b);
};

