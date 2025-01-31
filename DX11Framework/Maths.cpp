#include "Maths.h"

float Maths::Magnitude(Vector3 a)
{
    return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

Vector3 Maths::Normalise(Vector3 a)
{
    return Vector3(a.x / Magnitude(a), a.y / Magnitude(a), a.z / Magnitude(a));
}

float Maths::Dot(Vector3 a, Vector3 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}
