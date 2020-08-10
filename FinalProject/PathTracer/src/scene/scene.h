#pragma once
#include <QList>
#include <raytracing/film.h>
#include <scene/camera.h>
#include <scene/thinlenscamera.h>
#include <scene/lights/light.h>
#include <scene/photonmap.h>

class Primitive;
class Material;
class Light;
class PhotonMap;

class Scene
{
public:
    Scene();
    ~Scene();
    QList<std::shared_ptr<Primitive>> primitives;
    QList<std::shared_ptr<Material>> materials;
    QList<std::shared_ptr<Light>> lights;
    Camera camera;
    //ThinLensCamera camera;
    Film film;
    PhotonMap* pm;
    void SetCamera(const Camera &c);

    void CreateTestScene();
    void Clear();

    void clearPM();

    bool Intersect(const Ray& ray, Intersection* isect) const;
};
