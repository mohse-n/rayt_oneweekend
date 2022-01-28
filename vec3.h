#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>


using std::sqrt;

class vec3
{
public:
    double e[3];

public:
    /* e{0,0,0} is the initializer list. */
    /* In case the constructor is called with no arguments. */
    vec3() : e{0,0,0} {}
    vec3(double e0,double e1,double e2) : e{e0,e1,e2} {}
    /* const methods cannot modify the object. 
    Example: double x() const {e[0]=5;} will result in an error. */
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const {return vec3(-e[0],-e[1],-e[2]);}
    /* What is this? */
    double operator[](int i) const {return e[i];}
    double& operator[](int i) {return e[i];} 

    vec3& operator+=(const vec3 &v){
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];

        return *this;
    }

    vec3& operator*=(const double t){
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;

        return *this;
    }

    vec3& operator/=(const double t){
        // e[0] *= t;
        // e[1] *= t;
        // e[2] *= t;
        /* We can be clever and use *= overloaded operator. */
        return *this *= 1/t;
    }

    double length() const{
        return sqrt(length_squared());
    }

    double length_squared() const{
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    /* static functions can be called even if there are no instances of the class, using only
    the class name. */
    /* Random point inside a unit box. */
    inline static vec3 random(){
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max){
        return vec3(random_double(min,max),random_double(min,max),random_double(min,max));
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(e[0]<s) && fabs(e[1]<s) && fabs(e[2]<s));
    }


};

using point3 = vec3; // 3D point
using color = vec3; // RGB color


/* vec3 utility functions */
inline std::ostream& operator<<(std::ostream &out, const vec3 &v){
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

/* Return a new object that is the addition of the two. */
inline vec3 operator+(const vec3 &u, const vec3&v){
    return vec3(u.e[0]+v.e[0],u.e[1]+v.e[1],u.e[2]+v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3&v){
    return vec3(u.e[0]-v.e[0],u.e[1]-v.e[1],u.e[2]-v.e[2]);
}

/* 3 possibilities for * operator. */
inline vec3 operator*(const vec3 &u, const vec3&v){
    return vec3(u.e[0]*v.e[0],u.e[1]*v.e[1],u.e[2]*v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v){
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3&v, double t){
    return t*v;
}

inline vec3 operator/(const vec3 &v, double t){
    return (1/t)*v;
}

inline double dot(const vec3 &u, const vec3 &v){
    return u.e[0]*v.e[0] + u.e[1]*v.e[1] + u.e[2]*v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v){
    return v / v.length();
}

 /* Rejection method: sample a unit box, check if the point is inside a unit sphere, 
    otherwise continue sampling. */
/* Note: given the distribution of the points, the sampling is not uniform. More points (higher probability)
close to the normal. */
vec3 random_in_unit_sphere(){
    while (true) {
        auto p = vec3::random(-1,1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

/* Results in a more uniform distribution. See Section 8.5 of the book. 
Instead of selecting random points inside the sphere, we find random points on the surface,
and use the direction from the center to the point along with an offset to get the bounced ray.*/
vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

/* Simple geometry using vector projection on the normal. */
vec3 reflect(const vec3& v, const vec3& n) {
    return v-2*dot(v,n)*n;
}

/* uv (first argument) is the incoming ray. */
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv,n),1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0-r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif