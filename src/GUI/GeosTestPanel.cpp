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

#include <webAsmPlay/Util.h>
#include <webAsmPlay/GUI/GUI.h>
#include <webAsmPlay/GUI/ImguiInclude.h>
#include <webAsmPlay/GeosTestCanvas.h>

void GUI::geosTestPanel()
{
    geosTestCanvas->setEnabled(showSceneViewPanel);

    if(!showSceneViewPanel) { return ;}
    
    ImGui::Begin("Geos Tests", &showSceneViewPanel);

        const ImVec2 pos = ImGui::GetCursorScreenPos();

        const ImVec2 sceneWindowSize = ImGui::GetWindowSize();

        geosTestCanvas->setArea(__(pos), __(sceneWindowSize));

        geosTestCanvas->setWantMouseCapture(GImGui->IO.WantCaptureMouse);

        ImGui::GetWindowDrawList()->AddImage(   (void *)geosTestCanvas->render(),
                                                pos,
                                                ImVec2(pos.x + sceneWindowSize.x, pos.y + sceneWindowSize.y),
                                                ImVec2(0, 1),
                                                ImVec2(1, 0));
        
        static float buffer1 = 0.1;
        static float buffer2 = 0.02;
        static float buffer3 = 0.22;

        ImGui::SliderFloat("buffer1", &buffer1, 0.0f, 0.3f, "buffer1 = %.3f");
        ImGui::SliderFloat("buffer2", &buffer2, 0.0f, 0.3f, "buffer2 = %.3f");
        ImGui::SliderFloat("buffer3", &buffer3, 0.0f, 0.3f, "buffer3 = %.3f");

        geosTestCanvas->setGeomParameters(buffer1, buffer2, buffer3);

    ImGui::End();
}

