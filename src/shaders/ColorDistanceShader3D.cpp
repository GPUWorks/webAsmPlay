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

#include <glm/gtc/type_ptr.hpp>
#include <webAsmPlay/Util.h>
#include <webAsmPlay/Types.h>
#include <webAsmPlay/Textures.h>
#include <webAsmPlay/shaders/ShaderProgram.h>
#include <webAsmPlay/shaders/ColorDistanceShader3D.h>

using namespace std;
using namespace glm;

// TODO A lot of code duplication! 

namespace
{
    ShaderProgram * shaderProgram = NULL;

    ColorDistanceShader3D * defaultInstance = NULL;

    GLint colorsInLoc = -1;

    GLint colorLookupOffsetLoc = -1;

    GLint heightMultiplierLoc = -1;

    GLint normalInLoc = -1;

    GLint modelLoc = -1;
    GLint viewLoc = -1;
    GLint projectionLoc = -1;

    //GLint MV_Uniform = -1;

    GLuint texUniform = -1;

    GLuint colorTexture = 0;

    glm::vec4 initalColors[32];
}

void ColorDistanceShader3D::ensureShader()
{
    // Shader sources
    const GLchar* vertexSource = R"glsl(#version 150 core

        uniform sampler2D tex; // TODO why does the ordering matter here? Something must not be correct.

        in vec3 vertIn;
        in vec3 normalIn;
        in float vertColorIn;
        
        uniform mat4   model;
        uniform mat4   view;
        uniform mat4   projection;
        uniform float  colorLookupOffset;
        uniform float  heightMultiplier;
        
        out vec4 vertexColorNear;
        out vec4 vertexColorFar;
        out vec4 position_in_view_space;
        out vec3 normal;
        out vec3 fragPos;

        void main()
        {
            vec4 vert = vec4(vertIn.xy, vertIn.z * heightMultiplier, 1);

            fragPos = vec3(model * vert);

            mat4 MV = view * model;

            position_in_view_space = MV * vert;

            gl_Position = projection * MV * vert;

            vertexColorNear = texture(tex, vec2(vertColorIn + colorLookupOffset / 32.0, 0.5));
            vertexColorFar  = texture(tex, vec2(vertColorIn + (1.0 + colorLookupOffset) / 32.0, 0.5));

            normal = mat3(transpose(inverse(model))) * normalIn;
        }
    )glsl";

    const GLchar* fragmentSource = R"glsl(#version 150 core
        
        in vec4 vertexColorNear;
        in vec4 vertexColorFar;
        in vec4 position_in_view_space;
        in vec3 normal; 
        in vec3 fragPos;

        out vec4 outColor;

        void main()
        {
            float minDist = 0.0;
            float maxDist = 5.0;

            vec3 lightPos = vec3(0.1,0.1,0.1);
            vec3 lightColor = vec3(1,1,1);
            vec3 viewPos = vec3(0,0,0);

            // computes the distance between the fragment position 
            // and the origin (4th coordinate should always be 1 
            // for points). The origin in view space is actually 
            // the camera position.
            float dist = max(0.0, distance(position_in_view_space, vec4(0.0, 0.0, 0.0, 1.0)) + minDist);
            
            dist = min(maxDist, dist) / maxDist;

            //outColor = vertexColorNear * (1.0f - dist) + vertexColorFar * dist;
            vec4 color = vertexColorNear * (1.0f - dist) + vertexColorFar * dist;
            vec3 objectColor = vec3(color);

            // ambient
            float ambientStrength = 0.1;
            vec3 ambient = ambientStrength * lightColor;
                
            // diffuse 
            vec3 norm = normalize(normal);
            vec3 lightDir = normalize(lightPos - fragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;
            
            // specular
            float specularStrength = 0.5;
            vec3 viewDir = normalize(viewPos - fragPos);
            vec3 reflectDir = reflect(-lightDir, norm);  
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * lightColor;  
                
            vec3 result = (ambient + diffuse + specular) * objectColor;
            outColor = vec4(result, color.w);
        }
    )glsl";

    colorTexture = Textures::create(initalColors, 32);

    shaderProgram = ShaderProgram::create(  vertexSource,
                                            fragmentSource,
                                            StrVec({"MV",
                                                    "colorsIn",
                                                    "tex",
                                                    "colorLookupOffset",
                                                    "heightMultiplier",
                                                    "model",
                                                    "view",
                                                    "projection"}),
                                            StrVec({"normalIn"}));

    colorLookupOffsetLoc = shaderProgram->getUniformLoc("colorLookupOffset");

    //MV_Uniform  = shaderProgram->getUniformLoc("MV");

    colorsInLoc = shaderProgram->getUniformLoc("colorsIn");

    texUniform = shaderProgram->getUniformLoc("tex");

    heightMultiplierLoc = shaderProgram->getUniformLoc("heightMultiplier");

    modelLoc      = shaderProgram->getUniformLoc("model");
    viewLoc       = shaderProgram->getUniformLoc("view");
    projectionLoc = shaderProgram->getUniformLoc("projection");

    normalInLoc = shaderProgram->getAttributeLoc("normalIn");

    dmess("normalInLoc " << normalInLoc);

    defaultInstance = new ColorDistanceShader3D();
}

ColorDistanceShader3D::ColorDistanceShader3D() : Shader(shaderProgram)
{
    colors[0] = vec4(1,0,0,1);
    colors[1] = vec4(1,1,0,1);
    colors[2] = vec4(1,0,1,1);
    colors[3] = vec4(0,1,0,1);
    colors[4] = vec4(0,1,1,1);
    colors[5] = vec4(0,0,1,1);
    colors[6] = vec4(1,0,0,1);
    colors[7] = vec4(1,0,0,1);
}

ColorDistanceShader3D::~ColorDistanceShader3D()
{

}

ColorDistanceShader3D * ColorDistanceShader3D::getDefaultInstance() { return defaultInstance ;}

void ColorDistanceShader3D::bind(const mat4 & model,
                                 const mat4 & view,
                                 const mat4 & projection,
                                 const bool   isOutline)
{
    if(colorTextureDirty)
    {
        Textures::set1D(colorTexture, colors, 32);

        colorTextureDirty = false;
    }

    GL_CHECK(glActiveTexture(GL_TEXTURE0));

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, colorTexture));

    shaderProgram->bind(model, view, projection);

    //shaderProgram->setUniform(MV_Uniform, model * view);

    shaderProgram->setUniformi(texUniform, 0);

    shaderProgram->setUniformf(heightMultiplierLoc, heightMultiplier);

    shaderProgram->setUniform(modelLoc,      model);
    shaderProgram->setUniform(viewLoc,       view);
    shaderProgram->setUniform(projectionLoc, projection);

    shaderProgram->enableVertexAttribArray(3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);

    shaderProgram->enableColorAttribArray(1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    GL_CHECK(glEnableVertexAttribArray(normalInLoc));

    GL_CHECK(glVertexAttribPointer(normalInLoc, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))));

    if(isOutline) { shaderProgram->setUniformf(colorLookupOffsetLoc, 2.0f) ;}
    else          { shaderProgram->setUniformf(colorLookupOffsetLoc, 0.0f) ;}
}

vec4 ColorDistanceShader3D::setColor(const size_t index, const vec4 & color)
{
    colorTextureDirty = true;

    return colors[index] = color;
}

vec4 ColorDistanceShader3D::getColor(const size_t index) { return colors[index] ;}

vec4 & ColorDistanceShader3D::getColorRef(const size_t index) { return colors[index] ;}

void ColorDistanceShader3D::loadState(const JSONObject & dataStore)
{
    auto setVec4 = [&dataStore](const wstring & key, vec4 & color)->void
    {
        JSONObject::const_iterator i = dataStore.find(key);

        if(i != dataStore.end()) { color = i->second->AsVec4() ;}
    };

    char buf[1024];

    for(size_t i = 0; i < 32; ++i)
    {
        sprintf(buf, "attributeColor_%i", (int)i);

        setVec4(stringToWstring(buf), colors[i]);
    }

    colorTextureDirty = true;
}

void ColorDistanceShader3D::saveState(JSONObject & dataStore)
{
    char buf[1024];

    for(size_t i = 0; i < 32; ++i)
    {
        sprintf(buf, "attributeColor_%i", (int)i);

        dataStore[stringToWstring(buf)] = new JSONValue(colors[i]);
    }
}

float ColorDistanceShader3D::setHeightMultiplier(const float multiplier) { return heightMultiplier = multiplier ;}