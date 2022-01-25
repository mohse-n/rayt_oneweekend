#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "vec3.h"

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
        /* rec.p + rec.normal, gets us to the center of the tangent sphere,
        and then we add a random vector inside it. */
        point3 target = rec.p + rec.normal + random_unit_vector();
        /* Note: recursion is introduced here. */
        /* ray(rec.p,target-rec.p) goes from the intersection point on the surface of the 
        sphere to the random point inside the sphere. So it is the bounced ray. */
        /* Note: because of this, the scanlines at the bottom (where there can be a lot of bouncing)
        take much longer than those at the top. */
        return 0.5*ray_color(ray(rec.p,target-rec.p),world,depth-1);
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
    const int max_depth = 5;

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
            pixel_color += ray_color(r,world, max_depth);
            }

        write_color(std::cout,pixel_color, samples_per_pixel);
        
        }

    }

    std::cerr << "\nDone.\n";


}