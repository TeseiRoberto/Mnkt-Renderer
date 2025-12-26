
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
 * @macro MAX_VERTEX_SHADER_IN_PARAMS
 * Maximum number of additional parameters that can be passed to the vertex shader
*/
#define MAX_VERTEX_SHADER_IN_PARAMS     8


/**
 * @macro MAX_VERTEX_SHADER_OUT_PARAMS
 * Maximum number of additional parameters that can be outputted from the vertex shader,
 * such parameters are then passed as inputs to the fragment shader
*/
#define MAX_VERTEX_SHADER_OUT_PARAMS     8


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
*/
typedef union {
        int32_t         intData;
        uint32_t        uintData;
        float           floatData;

        Vec2_t          vec2;
        Vec3_t          vec3;
        Vec4_t          vec4;

        Image_t*        texture;

} ShaderParameter_t;


/**
 * @typedef VertexShaderFunc_t
 * Typedef for the function pointer data type that can be used as a vertex shader.
 * Such function must return a vector which components defines the final position of the vertex
*/
typedef Vec4_t (*VertexShaderFunc_t)(Vec3_t* inVertex, const ShaderParameter_t* const uniforms);


/**
 * @typedef FragmentShaderFunc_t
 * Typedef for the function pointer data type that can be used as a fragment shader.
 * Such function must return a vector which components defines the color of the fragment
*/
typedef Vec3_t (*FragmentShaderFunc_t)(const ShaderParameter_t* const uniforms);


/**
 * @struct ShaderProgram_t
 * Models a shader program that must be used to compute vertices and fragments data when a
 * draw operation is invoked.
*/
typedef struct {

        VertexShaderFunc_t      vertexShader;                                           ///< Function to be used as vertex shader
        ShaderParameter_t       vertexShaderInParams[MAX_VERTEX_SHADER_IN_PARAMS];      ///< Additional parameters to be passed as input to the vertex shader
        
        FragmentShaderFunc_t    fragmentShader;                                         ///< Function to be used as fragment shader
        ShaderParameter_t       fragmentShaderInParams[MAX_VERTEX_SHADER_OUT_PARAMS];   ///< Additional parameters to be passed as input to the fragment shader (outputted from vertex shader)

        ShaderParameter_t       uniforms[MAX_UNIFORM_PARAMS];                           ///< Uniform parameters to be passed to shader

} ShaderProgram_t;


#endif // MNKT_SHADER_H

