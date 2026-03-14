
/**
 * @file math4x4.h
 *
 * Defines a basic structure that models a four by four matrix and functions to operate on it.
 *
 * @warning Functions that operates on matrices takes them as pointers to avoid copies,
 * no check is made on the validity of such pointers!!!
*/

#ifndef MNKT_MAT4X4_H
#define MNKT_MAT4X4_H


#include <stdint.h>
#include <math.h>

#include "vec.h"


/**
 * @struct Mat4x4_t
 * Models a four by four matrix
*/
typedef struct {
        float   values[16];     ///< The values of the matrix stored by rows
} Mat4x4_t;


/**
 * @function mnkt_mat4x4_identity
 * @return An identity matrix
*/
Mat4x4_t        mnkt_mat4x4_identity();


/**
 * @function mnkt_mat4x4
 * @param value Value to be used to fill the diagonal of the matrix
 * @return A matrix filled with zeros and the given value on the diagonal
*/
Mat4x4_t        mnkt_mat4x4(float value);


Mat4x4_t        mnkt_mat4x4_sum(const Mat4x4_t* a, const Mat4x4_t* b);
Mat4x4_t        mnkt_mat4x4_sub(const Mat4x4_t* a, const Mat4x4_t* b);
Mat4x4_t        mnkt_mat4x4_mul(const Mat4x4_t* a, const Mat4x4_t* b);

Vec4_t          mnkt_mat4x4_vecMul(const Mat4x4_t* mat, const Vec4_t* vec);

Mat4x4_t        mnkt_mat4x4_scalarSum(const Mat4x4_t* mat, float value);
Mat4x4_t        mnkt_mat4x4_scalarSub(const Mat4x4_t* mat, float value);
Mat4x4_t        mnkt_mat4x4_scalarMul(const Mat4x4_t* mat, float value);
Mat4x4_t        mnkt_mat4x4_scalarDiv(const Mat4x4_t* mat, float value);


/**
 * @function mnkt_mat4x4_translation
 * @param x, y, z Coordinates towards which the translation must be performed
 * @return A translation matrix
*/
Mat4x4_t        mnkt_mat4x4_translation(float x, float y, float z);


/**
 * @function mnkt_mat4x4_translation
 * @param axis Axis around wich the rotation must be performed
 * @param angle Angle, expressed in radiants, of the rotation to be performed
 * @return A rotation matrix
*/
Mat4x4_t        mnkt_mat4x4_rotation(Vec3_t axis, float angle);


/**
 * @function mnkt_mat4x4_translation
 * @param x, y, z Values, relative to each axis, that must be used to perform the scaling
 * @return A scaling matrix
*/
Mat4x4_t        mnkt_mat4x4_scaling(float x, float y, float z);


#endif // MNKT_MAT4X4_H
