#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

/* Derived from hittable class. */
class sphere : public hittable {
    public:
        point3 center;
        double radius;

    sphere() {}
    sphere(point3 cen, double r) : center(cen), radius(r) {};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {

 vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    /* Check if the smaller root is within range. */
    if (root < t_min || root > t_max){
        /* If it's not, check the larger root. */
        root = (-half_b + sqrtd) / a;
        if (root < t_min || root > t_max)
            /* No hit point in the range of t. */
            return false;
    }

    /* Go with the smaller root. */
    rec.t = root;
    /* Position of the hit. */
    rec.p = r.at(rec.t);
    /* Normal at the hit. */
    vec3 outward_normal = (rec.p-center) /radius;
    /* Set the direction of the normal against the ray. */
    rec.set_face_normal(r,outward_normal);

    return true;
}

#endif