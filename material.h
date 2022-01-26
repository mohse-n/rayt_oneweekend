#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "hittable.h"

struct hit_record;

class material{
    public:
        virtual bool scatter (const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};


/* Diffuse materials. */
class lambertian : public material {
    public:
        color albedo;

    public:
        lambertian(const color& a) : albedo(a) {}

    /* When a derived class overrides a _virtual_ method, even if we use a pointer/reference to 
    the parent class, the method in the derived class will be used. */
    /* See: en.cppreference.com/w/cpp/language/virtual */
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        auto scatter_direction = rec.normal + random_unit_vector();
        /* Catch degenerate scatter direction (when normal & the random are opposite to each other)
        the scatter vector ~ 0. */
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        /* The scattered ray. */
        scattered = ray(rec.p,scatter_direction);
        attenuation = albedo;
        return true;
    }
    
};

/* Shiny materials. Note they ahve different scatter methods. */
class metal : public material {
    public:
        color albedo;

    public:
        metal(const color& a) : albedo(a) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

};




#endif