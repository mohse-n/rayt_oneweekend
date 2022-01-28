#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u,v,w;
        double lens_radius;

    public:
        camera(
        point3 lookfrom,
        point3 lookat,
        vec3 vup,
        double vfov, // vertical field-of-view in degrees
        double aspect_ratio,
        double aperture,
        double focus_dist
        ) {
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            /* Simple geometry. Relationship between field of view & height of the viewport. */
            auto viewport_height = 2.0*h;
            auto viewport_width = aspect_ratio * viewport_height;
            
            auto w = unit_vector(lookfrom - lookat);
            auto u = unit_vector(cross(vup,w));
            auto v = cross(w,u);

            auto focal_length = 1.0;

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

            lens_radius = aperture / 2;
        }

        /* Cast a ray to each pixel. 
            The start of the ray is (0,0,0) or origin hence -origin term.
            lower_left_corner+s*horizontal+t*vertical spans all the pixels. */
        ray get_ray(double s, double t) const {
            vec3 rd = lens_radius*random_in_unit_disk();
            vec3 offset = u*rd.x() + v*rd.y();

            return ray(
                origin+offset,
                lower_left_corner + s*horizontal + t*vertical - origin - offset
            );
        }
};

#endif