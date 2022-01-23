#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray{
    public:
        point3 orig;
        vec3 dir;

    public:
        ray() {}
        ray(const point3& origin, const vec3& direction): 
            orig(origin), dir(direction) {}

        /* Getter methods. */
        point3 origin() const {return orig;}
        vec3 direction() const {return dir;}

        /* Ray position along a 3d line. */
        point3 at(double t) const {
            return orig+t*dir;
        }

};


#endif