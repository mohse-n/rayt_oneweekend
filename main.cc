#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>

double hit_sphere(const point3& center, double radius, const ray& r){
    /* Vector from origin to the center of the circle. */
    vec3 oc = r.origin() - center;
    /* To check whether the ray hits the sphere or not, we have to check the discriminant of a quadratic equation. 
    To obtain the hit points, we have to solve it. */
    auto a = r.direction().length_squared();
    auto half_b = dot(oc,r.direction()); 
    auto c = oc.length_squared()-radius*radius;
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0){
        return -1.0;
    } else {
        return (-half_b-sqrt(discriminant)) / a;
    }
}

/* Return the color of the ray. */
color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    /* t_max = infinity. */
    if (world.hit(r,0,infinity,rec)){
        return 0.5*(rec.normal + color(1,1,1));
    }
    
    /* The background. */
    /* Get the direction of the ray. */
    vec3 unit_direction = unit_vector(r.direction());
    /* Parameterization to create a gradient for the background. 
    y changes from -1 to 1, so t changes from 0 to 1. */
    auto t = 0.5*(unit_direction.y() + 1.0);
    /* Linearly blend between while & 0.5 0.7 1.0. */
    return (1.0-t)*color(1.0,1.0,1.0)+t*color(0.5,0.7,1.0);
}

int main(){

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;

    // World
    hittable_list world;
    /* The small sphere. */
    world.add(make_shared<sphere>(point3(0,0,-1),0.5));
    /* The ground (a large sphere, only partially visible). */
    world.add(make_shared<sphere>(point3(0,-100.5,-1),100));

    // Camera
    camera cam;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j=image_height-1;j>=0;j--){
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i=0;i<image_width;i++){
            color pixel_color(0,0,0);
            /* Cast rays around each pixel. */
            for (int s=0;s<samples_per_pixel;s++){
            auto u = (i+random_double()) / (image_width-1);
            auto v = (j+random_double()) / (image_height-1);
        
            ray r = cam.get_ray(u,v);
            /* Calculate the color that we see. */
            pixel_color += ray_color(r,world);
            }

        write_color(std::cout,pixel_color, samples_per_pixel);
        
        }

    }

    std::cerr << "\nDone.\n";


}