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

#ifndef __WEB_ASM_PLAY_UTIL_H__
#define __WEB_ASM_PLAY_UTIL_H__

#ifdef OPENGL_CALL_CHECKING

    #ifdef __EMSCRIPTEN__
        // GLEW
        #define GLEW_STATIC
        #include <GL/glew.h>
    #else
        #include <GL/gl3w.h>
    #endif

#endif

#include <string>
#include <vector>
#include <chrono>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <OpenSteer/Vec3.h>
#include <OpenSteer/Color.h>
#include <webAsmPlay/Debug.h>
#include <webAsmPlay/GUI/GUI.h>
#include <imgui.h>

// TODO add util namespace

std::string toStr(const glm::mat4 & m);

std::string toStr(const glm::vec2 & v);

std::string toStr(const glm::vec3 & v);

std::string toStr(const glm::vec4 & v);

static glm::ivec2 __(const ImVec2 & v) { return glm::ivec2(v.x, v.y) ;}

static ImVec4 __(const glm::vec4 & v) { return ImVec4(v.x, v.y, v.z, v.q) ;}

static glm::vec4 __(const ImVec4 & v) { return glm::vec4(v.x, v.y, v.z, v.w) ;}

static glm::vec3 __(const OpenSteer::Vec3 & v) { return glm::vec3(v.x, v.y, v.z) ;}

static glm::vec4 __(const OpenSteer::Color & v) { return glm::vec4(v.r(), v.g(), v.b(), v.a()) ;}

std::wstring stringToWstring(const std::string& t_str);

void doProgress(const std::string                                    & message,
                const size_t                                         i,
                const size_t                                         num,
                std::chrono::time_point<std::chrono::system_clock> & startTime,
                const size_t                                         sampleRate = 1000);

#ifdef OPENGL_CALL_CHECKING
    #define GL_CHECK(stmt) do { stmt; \
            const GLenum err = glGetError(); \
            if (err != GL_NO_ERROR) { \
            dmess("OpenGL error " << err << " call " << #stmt); \
            abort(); \
            } \
        } while (0)
#else
    #define GL_CHECK(stmt) stmt
#endif

inline uint32_t getUint32(const char *& dataStream)
{
    const uint32_t ret = *(uint32_t *)dataStream; dataStream += sizeof(uint32_t);

    return ret;
}

inline uint32_t appendUint32(char *& dataStream, const uint32_t value)
{
    *(uint32_t *)dataStream = value;

    dataStream += sizeof(uint32_t);

    return value;
}

inline void appendUint32s(char *& dataStream, const uint32_t A, const uint32_t B, const uint32_t C)
{
    appendUint32(dataStream, A);
    appendUint32(dataStream, B);
    appendUint32(dataStream, C);
}

inline char * appendChar(std::vector<char> & data, const char value)
{
    char * ret = &data[0];

    *ret = value;

    return ret + 1;
}

inline double getDouble(const char *& dataStream)
{
    const double ret = *(double *)dataStream; dataStream += sizeof(double);

    return ret;
}

inline float append(float *& dataStream, const float value)
{
    *dataStream = value; 

    ++dataStream;

    return value;
}

inline uint32_t append(uint32_t *& dataStream, const uint32_t value)
{
    *dataStream = value; 

    ++dataStream;

    return value;
}

inline void append( uint32_t       *& dataStream,
                    const uint32_t    valueA,
                    const uint32_t    valueB)
{
    *dataStream = valueA; ++dataStream;
    *dataStream = valueB; ++dataStream;
}

inline void append( float       *& dataStream,
                    const float    valueA,
                    const float    valueB,
                    const float    valueC)
{
    *dataStream = valueA; ++dataStream;
    *dataStream = valueB; ++dataStream;
    *dataStream = valueC; ++dataStream;
}

inline glm::vec3 append(float *& dataStream, const glm::vec3 & value)
{
    memcpy(dataStream, value_ptr(value), sizeof(glm::vec3));

    dataStream += 3;

    return value;
}

inline glm::vec4 append2f(float *& dataStream, const glm::vec4 & value)
{
    *dataStream = value.x; ++dataStream;
    *dataStream = value.y; ++dataStream;

    return value;
}

#endif // __WEB_ASM_PLAY_UTIL_H__