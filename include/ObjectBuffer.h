#pragma once
#ifdef _WIN32
    #include <GL/glew.h>
#else
    #include <GLES3/gl32.h>
#endif
/** @struct ObjectBuffer
 * @brief Used for store `Object3D` buffers IDs
 */
struct ObjectBuffer {
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint VBOtexture = 0;
    GLuint VBOnormal = 0;
    GLuint VBOtangent = 0;
    GLuint EBO = 0;
};
