#include "squareplane.h"

float SquarePlane::Area() const
{
    //TODO
    float area = transform.getScale().x * transform.getScale().y;
    return area;
}

bool SquarePlane::Intersect(const Ray &ray, Intersection *isect) const
{
    //Transform the ray
    Ray r_loc = ray.GetTransformedCopy(transform.invT());

    //Ray-plane intersection
    float t = glm::dot(glm::vec3(0,0,1), (glm::vec3(0.5f, 0.5f, 0) - r_loc.origin)) / glm::dot(glm::vec3(0,0,1), r_loc.direction);
    Point3f P = Point3f(t * r_loc.direction + r_loc.origin);
    //Check that P is within the bounds of the square
    if(t > 0 && P.x >= -0.5f && P.x <= 0.5f && P.y >= -0.5f && P.y <= 0.5f)
    {
        InitializeIntersection(isect, t, P);
        return true;
    }
    return false;
}

void SquarePlane::ComputeTBN(const Point3f &P, Normal3f *nor, Vector3f *tan, Vector3f *bit) const
{
    *nor = glm::normalize(transform.invTransT() * Normal3f(0,0,1));
    CoordinateSystem(*nor, tan, bit);
}


Point2f SquarePlane::GetUVCoordinates(const Point3f &point) const
{
    return Point2f(point.x + 0.5f, point.y + 0.5f);
}

Intersection SquarePlane::Sample(const Point2f &xi, Float *pdf) const {
    // Generate a world-space point on the surface of the shape.
    glm::vec4 p = transform.T() * glm::vec4(xi.x - 0.5f, xi.y - 0.5f, 0.f, 1.f);
    // Create an Intersection to return.
    Intersection inter = Intersection();
    inter.point = Vector3f(p.x, p.y, p.z);
    // Set the point and normal of this Intersection to the correct values.
    inter.normalGeometric = glm::normalize(transform.invTransT() * Normal3f(0,0,1));
    // Set the output PDF to the correct value, which would be a uniform PDF with respect to surface area.
    *pdf = 1.f / Area();
    return inter;
}
