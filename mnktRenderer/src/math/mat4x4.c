
/**
 * @file math4x4.c
 *
 * Contains implementation of the four by four matrix API
*/

#include "mat4x4.h"


/**
 * @brief mnkt_mat4x4_identity
 * @return An identity matrix
*/
Mat4x4_t mnkt_mat4x4_identity()
{
        return (Mat4x4_t) {
                {
                        1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1
                }
        };
}


/**
 * @brief mnkt_mat4x4
 * @param value Value to be used to fill the matrix diagonal
 * @return A matrix filled with zeros and the given value on the diagonal
*/
Mat4x4_t mnkt_mat4x4(float value)
{
        return (Mat4x4_t) {
                {
                        value,  0,      0,      0,
                        0,      value,  0,      0,
                        0,      0,      value,  0,
                        0,      0,      0,      value
                }
        };
}


Mat4x4_t mnkt_mat4x4_sum(const Mat4x4_t* a, const Mat4x4_t* b)
{
        Mat4x4_t res;

        for(uint8_t i = 0; i < 16; ++i)
                res.values[i] = a->values[i] + b->values[i];

        return res;
}


Mat4x4_t mnkt_mat4x4_sub(const Mat4x4_t* a, const Mat4x4_t* b)
{
        Mat4x4_t res;

        for(uint8_t i = 0; i < 16; ++i)
                res.values[i] = a->values[i] - b->values[i];

        return res;
}


Mat4x4_t mnkt_mat4x4_mul(const Mat4x4_t* a, const Mat4x4_t* b)
{
        Mat4x4_t res;
        uint8_t resMatIndex = 0;

        for(uint8_t row = 0; row < 4; ++row)
        {
                const float* rowData = a->values + (row * 4);

                for(uint8_t col = 0; col < 4; ++col)
                {
                        float currValue = 0.0f;
                        const float* colData = b->values + col;

                        for(uint8_t i = 0; i < 4; ++i)
                                currValue += rowData[i] * colData[i * 4];

                        res.values[resMatIndex] = currValue;
                        ++resMatIndex;
                }
        }

        return res;
}


Vec4_t mnkt_mat4x4_vecMul(const Mat4x4_t* mat, const Vec4_t* vec)
{
        Vec4_t res;

        float* resVecData = ( (float*) &res );
        const float* vecData = ( (const float*) vec );

        for(uint8_t row = 0; row < 4; ++row)
        {
                resVecData[row] = 0.0f;

                for(uint8_t col = 0; col < 4; ++col)
                        resVecData[row] += mat->values[row * 4 + col] * vecData[col];
        }

        return res;
}


Mat4x4_t mnkt_mat4x4_scalarSum(const Mat4x4_t* mat, float value)
{
        Mat4x4_t res;

        for(uint8_t i = 0; i < 16; ++i)
                res.values[i] = mat->values[i] + value;

        return res;
}


Mat4x4_t mnkt_mat4x4_scalarSub(const Mat4x4_t* mat, float value)
{
        Mat4x4_t res;

        for(uint8_t i = 0; i < 16; ++i)
                res.values[i] = mat->values[i] - value;

        return res;
}


Mat4x4_t mnkt_mat4x4_scalarMul(const Mat4x4_t* mat, float value)
{
        Mat4x4_t res;

        for(uint8_t i = 0; i < 16; ++i)
                res.values[i] = mat->values[i] * value;

        return res;
}


Mat4x4_t mnkt_mat4x4_scalarDiv(const Mat4x4_t* mat, float value)
{
        Mat4x4_t res;

        for(uint8_t i = 0; i < 16; ++i)
                res.values[i] = mat->values[i] / value;

        return res;
}


/**
 * @brief mnkt_mat4x4_translation
 * @param x, y, z Coordinates towards which the translation must be performed
 * @return A translation matrix
*/
Mat4x4_t mnkt_mat4x4_translation(float x, float y, float z)
{
        Mat4x4_t res = mnkt_mat4x4_identity();

        res.values[3] = x;
        res.values[7] = y;
        res.values[11] = z;

        return res;
}


/**
 * @brief mnkt_mat4x4_translation
 * @param axis Axis around wich the rotation must be performed
 * @param angle Angle, expressed in radiants, of the rotation to be performed
 * @return A rotation matrix
*/
Mat4x4_t mnkt_mat4x4_rotation(Vec3_t axis, float angle)
{
        float sinTheta = sinf(angle);
        float cosTheta = cosf(angle);

        // Precomupte some values
        float oneMinusCos = 1.0f - cosTheta;

        float xy = axis.x * axis.y;
        float xz = axis.x * axis.z;
        float yz = axis.y * axis.z;

        // Compute matrix
        Mat4x4_t res;

        res.values[0]   = ((axis.x * axis.x) * oneMinusCos) + cosTheta;
        res.values[1]   = (xy * oneMinusCos) - axis.z * sinTheta;
        res.values[2]   = (xz * oneMinusCos) + axis.y * sinTheta;
        res.values[3]   = 0.0f;

        res.values[4]   = (xy * oneMinusCos) + axis.z * sinTheta;
        res.values[5]   = ((axis.y * axis.y) * oneMinusCos) + cosTheta;
        res.values[6]   = (yz * oneMinusCos) - axis.x * sinTheta;
        res.values[7]   = 0.0f;

        res.values[8]   = (xz * oneMinusCos) - axis.y * sinTheta;
        res.values[9]   = (yz * oneMinusCos) + axis.x * sinTheta;
        res.values[10]  = ((axis.z * axis.z) * oneMinusCos) + cosTheta;
        res.values[11]  = 0.0f;

        res.values[12]  = 0.0f;
        res.values[13]  = 0.0f;
        res.values[14]  = 0.0f;
        res.values[15]  = 1.0f;

        return res;
}


/**
 * @brief mnkt_mat4x4_translation
 * @param x, y, z Values, relative to each axis, that must be used to perform the scaling
 * @return A scaling matrix
*/
Mat4x4_t mnkt_mat4x4_scaling(float x, float y, float z)
{
        Mat4x4_t res = mnkt_mat4x4_identity();

        res.values[0] = x;
        res.values[5] = y;
        res.values[10] = z;

        return res;
}


