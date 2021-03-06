#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "vec3.h"
#include "material.h"

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
color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered. 
    if (depth <= 0)
        return color(0,0,0);

    /* t_max = infinity. */
    /* 0.001: ignore hits very near zero. (to fix the shadow acne problem) */
    if (world.hit(r,0.001,infinity,rec)){
        ray scattered;
        color attenuation;
        /* Note: recursion is introduced here. */
        /* ray(rec.p,target-rec.p) goes from the intersection point on the surface of the 
        sphere to the random point inside the sphere. So it is the bounced ray. */
        /* Note: because of this, the scanlines at the bottom (where there can be a lot of bouncing)
        take much longer than those at the top. */
        if (rec.mat_ptr->scatter(r,rec,attenuation,scattered))
            return attenuation*ray_color(scattered,world,depth-1);
        return color(0,0,0);
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
    const int max_depth = 50;

    // World
    hittable_list world;
    
    auto material_ground = make_shared<lambertian>(color(0.8,0.8,0));
    auto material_center = make_shared<lambertian>(color(0.1,0.2,0.5));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(color(0.8,0.6,0.2),0.0);

    /* Large sphere: ground */
    world.add(make_shared<sphere>(point3(0.0,-100.5,-1.0),100.0,material_ground));
    world.add(make_shared<sphere>(point3(0.0,0.0,-1.0),0.5,material_center));
    world.add(make_shared<sphere>(point3(-1.0,0.0,-1.0),-0.4,material_left));
    world.add(make_shared<sphere>(point3(1.0,0.0,-1.0),0.5,material_right));

    // Camera
    point3 lookfrom(3,3,2);
    point3 lookat(0,0,-1);
    vec3 vup(0,1,0);
    auto dist_to_focus = (lookfrom-lookat).length();
    auto aperture = 2.0;

    // point3 lookfrom(0,0,0);
    // point3 lookat(0,0,-1);
    // vec3 vup(0,1,0);
    // auto dist_to_focus = (lookfrom-lookat).length();
    // auto aperture = 1.0;
    
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

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
                pixel_color += ray_color(r,world, max_depth);
            }

        write_color(std::cout,pixel_color, samples_per_pixel);
        
        }

    }

    std::cerr << "\nDone.\n";


}