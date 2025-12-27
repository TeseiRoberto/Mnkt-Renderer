
/**
 * @file shader.h
 *
 * Defines the ShaderProgram_t struct and all the stuff related to the shaders
*/


#ifndef MNKT_SHADER_H
#define MNKT_SHADER_H

#include <stdint.h>

#include "vec.h"


/**
 * @macro MAX_VARYING_PARAMS
 * Maximum number of additional parameters that can be outputted from the vertex shader,
 * such parameters are then passed as inputs to the fragment shader
*/
#define MAX_VARYING_PARAMS     8


/**
 * @macro MAX_UNIFORM_PARAMS
 * Maximum number of uniform parameters that can be set for a shader program
*/
#define MAX_UNIFORM_PARAMS              8


/**
 * @union ShaderParameter_t
 * Union of all data types that can be passed as parameters to a vertex/fragment shader.
 * Is responsibility of the user to ensure that the data type used in the shader and the
 * one setted from the user code do match.
 *
 * This is used to implement varyings and uniforms parameters.
*/
typedef union {
        int32_t         intData;
        uint32_t        uintData;
        float           floatData;

        Vec2_t          vec2;
        Vec3_t          vec3;
        Vec4_t          vec4;

        Image_t*        texture;

        void*           userData;

} ShaderParameter_t;


/**
 * @typedef VertexShaderFunc_t
 * Typedef for the function pointer data type that can be used as a vertex shader.
 *
 * Such function takes as input:
 *      - an array of varying parameters, here additional output values can be stored, those will be later passed as inputs to the fragment shader
 *      - an array of uniform parameters
 *
 * Such function must output a vector which defines the coordinates of the vertex in clip space
*/
typedef Vec4_t (*VertexShaderFunc_t)(const void* vertex, ShaderParameter_t* varyings, const ShaderParameter_t* uniforms);


/**
 * @typedef FragmentShaderFunc_t
 * Typedef for the function pointer data type that can be used as a fragment shader.
 *
 * Such function takes as input:
 *      - an array of varying parameters, those can be outputted by the vertex shader
 *      - an array of uniform parameters
 *      - the coordinates of the fragment to be processed, expressed in pixels
 *      - a flag which indicates if the fragment must be discared or not
 *
 * Such function must output a vector which defines the color of the fragment
*/
typedef Vec4_t (*FragmentShaderFunc_t)(ShaderParameter_t* varyings, const ShaderParameter_t* uniforms, const Vec2_t* fragCoords, int* discard);


/**
 * @struct ShaderProgram_t
 * Models a shader program that can be used during a draw operation
*/
typedef struct {
        VertexShaderFunc_t      vertexShader;                   ///< Function to be used as vertex shader
        FragmentShaderFunc_t    fragmentShader;                 ///< Function to be used as fragment shader

        size_t                  vertexSize;                     ///< Size in bytes of a single vertex (containing all the attributes necessary for one invocation of the vertex shader)

        ShaderParameter_t       uniforms[MAX_UNIFORM_PARAMS];   ///< Uniform parameters to be passed to shader

} ShaderProgram_t;



#endif // MNKT_SHADER_H

