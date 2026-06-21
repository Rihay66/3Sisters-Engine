Game Engine: 3Sisters
* Organization
- Seperation of system/modules: "low-level handling gets simplfied to a single library for multi use of any API or library like Raylib"
- Documentation: Through a single readme
- Resources:
+ Lua 
+ Pico-8
+ Pyxel
+ Love2D
+ Playdate SDK
+ DragonRuby Game Toolkit (DRGTK)
+ Usagi 
* Goals
+ Can do both "minimal"(Similar to Usagi or Pico-8) and use Lua 
+ "Advance" export dlls/libraries to be used by different languages
+ Tools: Scene inspector, Image looker, music tester, model look, shader test
+ Able to do "simple" 3D (Basic primatives, model loader)
+ Scene system
+ Modular to allow for additional tools with minimal integration (load/unload a built tool)
+ Export for Mac, Windows, Linux, Web 
+ Live reloading (With Lua, and possibly with other languages in "advance")
+ Blending both "spartan programming" and "Inspector"
* Tools/Libraries usage
+ SDL3 (input handling, audio, window management)
+ OpenGL & Emscripten (Copy most API from 3Sisters-Legacy)

Main Features to work on:
- Graphics/Input/Window/Audio handling (completely in C)
- LUA engine
- Live reloading LUA
- Arguments handling on app
- Emscripten
- Project setup

Things to do when base is done:
* 

Additional features after baseline done
- Builtin pause menu
- builtin meta data saving/loading
- Tools
- Plugins? (May need further looking into)