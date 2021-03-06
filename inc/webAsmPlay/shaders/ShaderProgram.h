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

#ifndef __WEB_ASM_PLAY_SHADER_PROGRAM_H__
#define __WEB_ASM_PLAY_SHADER_PROGRAM_H__

#ifdef __EMSCRIPTEN__
    // GLEW
    #define GLEW_STATIC
    #include <GL/glew.h>
#else
    #include <GL/gl3w.h>
#endif

#include <unordered_map>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <webAsmPlay/Types.h>

typedef std::vector<std::pair<std::string, GLint &> > Variables;

class ShaderProgram
{
public:

    static ShaderProgram * create(const GLchar    * vertexSource,
                                  const GLchar    * fragmentSource,
                                  const GLchar    * geometrySource   = NULL,
                                  const Variables & attributes       = Variables(),
                                  const Variables & uniforms         = Variables());

    static ShaderProgram * create(const GLchar    * vertexSource,
                                  const GLchar    * fragmentSource,
                                  const Variables & attributes,
                                  const Variables & uniforms = Variables());
                                  

    void bind();

    GLuint getProgramHandle() const;

    static void enableVertexAttribArray(const GLint       vertexLoc,
                                        const GLint       size        = 2,
                                        const GLenum      type        = GL_FLOAT,
                                        const GLboolean   normalized  = GL_FALSE,
                                        const GLsizei     stride      = 0,
                                        const GLvoid    * pointer     = NULL);

    GLint getUniformLoc  (const std::string & name) const;
    GLint getAttributeLoc(const std::string & name) const;

    void setUniform (const GLint location, const glm::mat4 & value) const;
    void setUniform (const GLint location, const glm::vec4 & value) const;
    void setUniformf(const GLint location, const float     & value) const;
    void setUniformi(const GLint location, const GLuint    & value) const;

private:

    ShaderProgram(const GLuint                                   shaderProgram,
                  const std::unordered_map<std::string, GLint> & uniforms,
                  const std::unordered_map<std::string, GLint> & attributes);

    ~ShaderProgram();

    GLuint shaderProgram;
    
    std::unordered_map<std::string, GLint> uniforms;
    std::unordered_map<std::string, GLint> attributes;
};

#endif // __WEB_ASM_PLAY_SHADER_PROGRAM_H__