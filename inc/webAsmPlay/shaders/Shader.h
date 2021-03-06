/**
 ╭━━━━╮╱╱╱╱╱╱╱╱╱╭╮╱╭━━━╮╱╱╱╱╱╱╭╮
 ┃╭╮╭╮┃╱╱╱╱╱╱╱╱╱┃┃╱┃╭━╮┃╱╱╱╱╱╱┃┃
 ╰╯┃┃╰╯╭━╮╭━━╮╭╮┃┃╱┃┃╱╰╯╭━━╮╭━╯┃╭━━╮
 ╱╱┃┃╱╱┃╭╯┃╭╮┃┣┫┃┃╱┃┃╱╭╮┃╭╮┃┃╭╮┃┃┃━┫
 ╱╱┃┃╱╱┃┃╱┃╭╮┃┃┃┃╰╮┃╰━╯┃┃╰╯┃┃╰╯┃┃┃━┫
 ╱╱╰╯╱╱╰╯╱╰╯╰╯╰╯╰━╯╰━━━╯╰━━╯╰━━╯╰━━╯
 // This software is provided 'as-is', without any express or implied
 // warranty.  In no event will the authors be held liable for any damages
 // arising from the use of this software.
 // Permission is granted to anyone to use this software for any purpose,
 // including commercial applications, and to alter it and redistribute it
 // freely, subject to the following restrictions:
 // 1. The origin of this software must not be misrepresented; you must not
 //    claim that you wrote the original software. If you use this software
 //    in a product, an acknowledgment in the product documentation would be
 //    appreciated but is not required.
 // 2. Altered source versions must be plainly marked as such, and must not be
 //    misrepresented as being the original software.
 // 3. This notice may not be removed or altered from any source distribution.

  \author Matthew Tang
  \email trailcode@gmail.com
  \copyright 2018
*/

#ifndef __WEB_ASM_PLAY_SHADER_H__
#define __WEB_ASM_PLAY_SHADER_H__

#ifdef __EMSCRIPTEN__
    // GLEW
    #define GLEW_STATIC
    #include <GL/glew.h>
#else
    #include <GL/gl3w.h>
#endif

#include <glm/mat4x4.hpp>

class ShaderProgram;
class Canvas;

class Shader
{
public:

    virtual void bind(Canvas * canvas, const bool isOutline) = 0;

    void enableVertexArray(const GLint       size        = 2,
                           const GLenum      type        = GL_FLOAT,
                           const GLboolean   normalized  = GL_FALSE,
                           const GLsizei     stride      = 0,
                           const GLvoid    * pointer     = NULL);

    void enableColorArray( const GLint       size        = 2,
                           const GLenum      type        = GL_FLOAT,
                           const GLboolean   normalized  = GL_FALSE,
                           const GLsizei     stride      = 0,
                           const GLvoid    * pointer     = NULL);

    void enableNormalArray( const GLint       size        = 2,
                            const GLenum      type        = GL_FLOAT,
                            const GLboolean   normalized  = GL_FALSE,
                            const GLsizei     stride      = 0,
                            const GLvoid    * pointer     = NULL);

    std::string getName() const;

protected:

    Shader( const std::string & shaderName,
            ShaderProgram     * program,
            const GLint         vertexInLoc = -1,
            const GLint         colorInLoc  = -1,
            const GLint         normalInLoc = -1);

    virtual ~Shader() {}

    const std::string shaderName;

    ShaderProgram * program;

    const GLint vertexInLoc;
    const GLint colorInLoc;
    const GLint normalInLoc;
};

#endif // __WEB_ASM_PLAY_SHADER_H__