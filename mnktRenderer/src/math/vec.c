
/*
 * @file vec.c
 *
 * Contains implementation of the vectors API
*/


#include "vec.h"
#include "mathUtils.h"


Vec2_t mnkt_vec2_add(const Vec2_t* a, const Vec2_t* b)
{
        return (Vec2_t)
        {
                .x = a->x + b->x,
                .y = a->y + b->y
        };
}


Vec2_t mnkt_vec2_sub(const Vec2_t* a, const Vec2_t* b)
{
        return (Vec2_t)
        {
                .x = a->x - b->x,
                .y = a->y - b->y
        };
}

Vec2_t mnkt_vec2_mul(const Vec2_t* a, const float scalar)
{
        return (Vec2_t)
        {
                .x = a->x * scalar,
                .y = a->y * scalar
        };
}


Vec2_t mnkt_vec2_div(const Vec2_t* a, const float scalar)
{
        return (Vec2_t)
        {
                .x = a->x / scalar,
                .y = a->y / scalar
        };
}


float mnkt_vec2_dot(const Vec2_t* a, const Vec2_t* b)
{
        return (a->x * b->x) + (a->y * b->y);
}


Vec2_t mnkt_vec2_clamp(const Vec2_t* a, const Vec2_t min, const Vec2_t max)
{
        return (Vec2_t) {
                .x = mnkt_math_clamp(a->x, min.x, max.x),
                .y = mnkt_math_clamp(a->y, min.y, max.y)
        };
}


Vec3_t mnkt_vec3_add(const Vec3_t* a, const Vec3_t* b)
{
        return (Vec3_t)
        {
                .x = a->x + b->x,
                .y = a->y + b->y,
                .z = a->z + b->z
        };
}


Vec3_t mnkt_vec3_sub(const Vec3_t* a, const Vec3_t* b)
{
        return (Vec3_t)
        {
                .x = a->x + b->x,
                .y = a->y + b->y,
                .z = a->z + b->z
        };
}


Vec3_t mnkt_vec3_mul(const Vec3_t* a, const float scalar)
{
        return (Vec3_t)
        {
                .x = a->x * scalar,
                .y = a->y * scalar,
                .z = a->z * scalar
        };
}


Vec3_t mnkt_vec3_div(const Vec3_t* a, const float scalar)
{
        return (Vec3_t)
        {
                .x = a->x / scalar,
                .y = a->y / scalar,
                .z = a->z / scalar
        };
}


float mnkt_vec3_dot(const Vec3_t* a, const Vec3_t* b)
{
        return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}


Vec3_t mnkt_vec3_cross(const Vec3_t* a, const Vec3_t* b)
{
        return (Vec3_t)
        {
                .x = (a->y * b->z) - (a->z * b->y),
                .y = (a->z * b->x) - (a->x * b->z),
                .z = (a->x * b->y) - (a->y * b->x),
        };

}


Vec3_t mnkt_vec3_clamp(const Vec3_t* a, const Vec3_t min, const Vec3_t max)
{
        return (Vec3_t) {
                .x = mnkt_math_clamp(a->x, min.x, max.x),
                .y = mnkt_math_clamp(a->y, min.y, max.y),
                .z = mnkt_math_clamp(a->z, min.z, max.z)
        };
}


Vec4_t mnkt_vec4_add(const Vec4_t* a, const Vec4_t* b)
{
        return (Vec4_t)
        {
                .x = a->x + b->x,
                .y = a->y + b->y,
                .z = a->z + b->z,
                .w = a->w + b->w
        };
}


Vec4_t mnkt_vec4_sub(const Vec4_t* a, const Vec4_t* b)
{
        return (Vec4_t)
        {
                .x = a->x - b->x,
                .y = a->y - b->y,
                .z = a->z - b->z,
                .w = a->w - b->w
        };
}


Vec4_t mnkt_vec4_mul(const Vec4_t* a, const float scalar)
{
        return (Vec4_t)
        {
                .x = a->x * scalar,
                .y = a->y * scalar,
                .z = a->z * scalar,
                .w = a->w * scalar
        };
}


Vec4_t mnkt_vec4_div(const Vec4_t* a, const float scalar)
{
        return (Vec4_t)
        {
                .x = a->x / scalar,
                .y = a->y / scalar,
                .z = a->z / scalar,
                .w = a->w / scalar
        };
}


float mnkt_vec4_dot(const Vec4_t* a, const Vec4_t* b)
{
        return (a->x * b->x) + (a->y * b->y) + (a->z * b->z) + (a->w * b->w);
}


Vec4_t mnkt_vec4_clamp(const Vec4_t* a, const Vec4_t min, const Vec4_t max)
{
        return (Vec4_t) {
                .x = mnkt_math_clamp(a->x, min.x, max.x),
                .y = mnkt_math_clamp(a->y, min.y, max.y),
                .z = mnkt_math_clamp(a->z, min.z, max.z),
                .w = mnkt_math_clamp(a->w, min.w, max.w)
        };
}
