#include "color.h"
#include "ray.h"
#include "vec3.h"


#include <iostream>

bool hit_sphere(const point3& center, double radius, const ray& r){
    /* Vector from origin to the center of the circle. */
    vec3 oc = r.origin() - center;
    /* To check whether the ray hits the sphere or not, we have to check the discriminant of a quadratic equation. 
    To obtain the hit points, we have to solve it. */
    auto a = dot(r.direction(),r.direction());
    auto b = 2.0*dot(oc,r.direction());
    auto c = dot(oc,oc)-radius*radius;
    auto discriminant = b*b - 4*a*c;
    if (discriminant < 0){
        return -1.0;
    } else {
        return (-b-sqrt(discriminant)) / (2.0*a);
    }
}

/* Return the color of the ray. */
color ray_color(const ray& r) {
    /* Obtain the hit points of the ray with the sphere. */
    auto sphere_center = point3(0,0,-1); 
    auto sphere_rad = 0.5;
    auto t = hit_sphere(sphere_center,sphere_rad,r);
    if (t > 0.0){
        /* Normal is the vector from the center of the sphere to the intersection point on the surface. */
        vec3 N = unit_vector(r.at(t)-sphere_center);
        /* Color the sphere. */
        return 0.5*color(N.x()+1,N.y()+1,N.z()+1);
    }
    /* Get the direction of the ray. */
    vec3 unit_direction = unit_vector(r.direction());
    /* Parameterization to create a gradient. 
    y changes from -1 to 1, to t changes from 0 to 1. */
    auto t = 0.5*(unit_direction.y() + 1.0);
    /* Linearly blend between while & 0.5 0.7 1.0. */
    return (1.0-t)*color(1.0,1.0,1.0)+t*color(0.5,0.7,1.0);
}

int main(){

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio*viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0,0,0);
    auto horizontal = vec3(viewport_width,0,0);
    auto vertical = vec3(0,viewport_height,0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0,0,focal_length);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j=image_height-1;j>=0;j--){
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i=0;i<image_width;i++){
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            /* Cast a ray to each pixel. 
            The start of the ray is (0,0,0) or origin hence -origin term.
            lower_left_corner+u*horizontal+v*vertical spans all the pixels. */
            ray r(origin,lower_left_corner+u*horizontal+v*vertical-origin);
            /* Calculate the color that we see. */
            color pixel_color = ray_color(r);
            write_color(std::cout,pixel_color);
        }

    }

    std::cerr << "\nDone.\n";


}