#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "rtweekend.h"

/* This just tells the compiler that the shared_ptr<material> is a pointer to a class. */
class material;

struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal){
        /* If the ray is in the opposite direction of the normal, it is coming fron outside to inside. */
        front_face = dot(r.direction(),outward_normal) < 0;
        /* Define the normal such that it always points against the ray. */
        normal = front_face ? outward_normal : - outward_normal;
    }
};

class hittable {
    public:
        /* A virtual function is used in the base class to enture that it will be overridden in a derived class.
        So it's really just like a prototype. */
        /* virtual func() = 0: pure virtual function, which means that it cannot be implement by the base (this) class. 
        When a pure virtual method exists, the class is "abstract" & cannot be instantiated on its own. */
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    
};

#endif