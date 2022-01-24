#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_record {
    point3 p;
    vec3 normal;
    double t;
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