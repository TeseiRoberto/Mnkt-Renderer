
/**
 * @file vec.h
 *
 * Defines basic structures which models mathemathical vectors
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


#endif // MNKT_VEC_H
