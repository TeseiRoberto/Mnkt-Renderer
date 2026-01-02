
/**
 * @file vec.h
 *
 * Defines basic structures which models mathemathical vectors
 * and functions to operate on them.
 *
 * @warning Functions that operates on vectors takes them as pointers to avoid copies,
 * no check is made on the validity of such pointers!!!
*/


#ifndef MNKT_VEC_H
#define MNKT_VEC_H


/**
 * @struct Vec2_t
 * Models a vector with two floating point components
*/
typedef struct {
        float   x;
        float   y;
} Vec2_t;


/**
 * @struct Vec3_t
 * Models a vector with three floating point components
*/
typedef struct {

        union {
                float   x;
                float   r;
        };

        union {
                float   y;
                float   g;
        };

        union {
                float   z;
                float   b;
        };

} Vec3_t;


/**
 * @struct Vec4_t
 * Models a vector with four floating point components
*/
typedef struct {

        union {
                float   x;
                float   r;
        };

        union {
                float   y;
                float   g;
        };

        union {
                float   z;
                float   b;
        };

        union {
                float   w;
                float   a;
        };

} Vec4_t;


Vec2_t  mnkt_vec2_add(const Vec2_t* a, const Vec2_t* b);
Vec2_t  mnkt_vec2_sub(const Vec2_t* a, const Vec2_t* b);
Vec2_t  mnkt_vec2_mul(const Vec2_t* a, const float scalar);
Vec2_t  mnkt_vec2_div(const Vec2_t* a, const float scalar);
float   mnkt_vec2_dot(const Vec2_t* a, const Vec2_t* b);
Vec2_t  mnkt_vec2_clamp(const Vec2_t* a, const Vec2_t min, const Vec2_t max);


Vec3_t  mnkt_vec3_add(const Vec3_t* a, const Vec3_t* b);
Vec3_t  mnkt_vec3_sub(const Vec3_t* a, const Vec3_t* b);
Vec3_t  mnkt_vec3_mul(const Vec3_t* a, const float scalar);
Vec3_t  mnkt_vec3_div(const Vec3_t* a, const float scalar);
float   mnkt_vec3_dot(const Vec3_t* a, const Vec3_t* b);
Vec3_t  mnkt_vec3_cross(const Vec3_t* a, const Vec3_t* b);
Vec3_t  mnkt_vec3_clamp(const Vec3_t* a, const Vec3_t min, const Vec3_t max);


Vec4_t  mnkt_vec4_add(const Vec4_t* a, const Vec4_t* b);
Vec4_t  mnkt_vec4_sub(const Vec4_t* a, const Vec4_t* b);
Vec4_t  mnkt_vec4_mul(const Vec4_t* a, const float scalar);
Vec4_t  mnkt_vec4_div(const Vec4_t* a, const float scalar);
float   mnkt_vec4_dot(const Vec4_t* a, const Vec4_t* b);
Vec4_t  mnkt_vec4_clamp(const Vec4_t* a, const Vec4_t min, const Vec4_t max);


#endif // MNKT_VEC_H
