#pragma once
#include "geometry/primitive.h"
#include "scene/scene.h"
#include "samplers/sampler.h"
#include "warpfunctions.h"


class Scene;
class Primitive;
class Bounds3f;
class Light;

typedef struct Photon {
    Photon() : wi(Point3f(),Vector3f()), objectHit(nullptr), worldnormal(), color(){}
    Photon(const Ray& incidentray, const Primitive* objhit, const Normal3f& n, const Color3f& pcolor)
        : wi(incidentray), objectHit(objhit), worldnormal(n), color(pcolor), children{nullptr, nullptr}, splitaxis(0) {}
    Ray wi;
    const Primitive* objectHit;
    Normal3f worldnormal;
    Color3f color;
    Photon *children[2];//0 is less, 1 is greater
    int splitaxis;
} Photon;



enum PHOTONTYPE {DIRECT, INDIRECT, CAUSTIC};

class PhotonMap
{
public:
    PhotonMap();
    PhotonMap(Scene*);
    Bounds3f WorldBound() const;
    ~PhotonMap();

    Photon* recursivelyMakeNodes(const int start, const int end, std::vector<Photon>& photons, const Bounds3f& bounds, int* totalNodes);
    Bounds3f getBounds(const std::vector<Photon>& photons);
    void emitPhotons(std::shared_ptr<Light> mylight);
    void addPhoton(std::vector<Photon>& photons, const Intersection& isect, const Ray& woWray, const Color3f& photoncolor);
    void traceUntilNotSpecular(Intersection& isect, Color3f& photoncolor, Color3f& throughput, Ray& woWray, int& spechits, bool& continueflag);
    void kNearestNeighborsInPlane(const PHOTONTYPE type, std::vector<Photon*>& knearestphotons, const Intersection& isect, const float MAXSQRDDIST, const int MAXPHOTONS, const bool isSSSPM);
    void kNNInPlane(Photon* const p, float& worstdist, std::vector<Photon*>& knnphotons, const Intersection& isect, const float MAXSQRDDIST, const float MAXPHOTONS,const bool isSSSPM );

    //members
    std::vector<Photon> directphotons;
    std::vector<Photon> indirectphotons;
    std::vector<Photon> causticphotons;
    const float inv_targetphotonpower = 1.f / 0.001f;
    const int maxbounces = 10;
    const float photonpowerweight = 20;
    const float artificialcausticscaling = 2;
   Scene* scene;
    Sampler* sampler;
    Photon* directnodes = nullptr;
    Photon* indirectnodes = nullptr;
    Photon* causticnodes = nullptr;
};
