----------------------
WebAsmPlay
----------------------

WebAsmPlay was initially developed to evaluate web application development using Emscripten.

Currently a handful of C++ libraries have been compiled with Emscripten to run in the browser and run natively. The idea is to test a cross platform development environment where an application is developed and debugged both as a web application and a browser application. Another goal is to discover the limitations and capabilities of Emscipten web application development. Cross platform development is achieved using cmake.

After initial evaluation the next goal of this project is to create a Real-TIme Strategy game using OpenSteer (http://opensteer.sourceforge.net/)  and OpenStreetMap (https://www.openstreetmap.org) To provide a starting point for non player character(NPC) zombie bots who roam Open Street Map cities. NPC positions will be calculated on a server and the client application running in the browser or natively can query bot positions and states for the given viewport. The server will also be responsible for geometry generalization, bot AI, navigation graphs generated from Open Street Map data and state persistence. 

#### Dependencies

Unfortunately during development notes on build depends were not tracked. Development is currently being don on OSX. Macports and Brew is being used to install third party dependencies. 

* Cmake: https://cmake.org
* Emscripten: http://kripken.github.io/emscripten-site
* glfw3
* Boost
* websocketpp
* gdal

Some third party dependencies are built into the WebAsmPlay source tree to make emscripten compilation easer:  
* Geos https://trac.osgeo.org/geos
* OpenSteer http://opensteer.sourceforge.net
* Ctpl https://github.com/vit-vit/CTPL
* Glm https://glm.g-truc.net/0.9.9/index.html
* GLUTesselator https://github.com/mbebenita/GLUTesselator
* Native Dear Imgui https://github.com/ocornut/imgui
* Emscripten Dear Imgui https://github.com/greggman/doodles/tree/master/glfw-imgui

#### Building

