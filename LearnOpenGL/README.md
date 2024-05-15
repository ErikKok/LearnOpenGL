TO DO
- Calling glUniform is quite expensive during run-time. Structure your programme so that glUniform is only called when the value needs to change. This might be the case every time that you draw a new object (e.g. its position might be different), but some uniforms may not change often (e.g. projection matrix).
- Logging https://antongerdelan.net/opengl/glcontext2.html
- Uniform location cache
- Model, BufferSubData, VertexAttribute, ShaderStorageBuffer .cpp maken/bijwerken
- GL_SHADER_STORAGE_BUFFER class maken in buffers.h
- 1 GL_SHADER_STORAGE_BUFFER met meerdere objecten/vectors erin mogelijk?
- time class maken
- light class maken
-  * light.strength wel op de goede plek in de shader/functie?
- avoid using vec3 at all https://stackoverflow.com/questions/47616015/opengl-and-glsl-memory-alignment-for-uniforms-and-varyings
- energy conservation, see comments at Blinn-Phong
- Blinn-Phong can also produce invalid highlights for objects that are lit from behind, see comments at Blinn-Phong
- which attenuation values/formula is best/sufficient in combination with gamma correction
- refactor to use DSA
- refactor to use create instead of gen and bind
- implement Bindless Textures
- implement immutable textures
- make debugQuad toggable

v0.1.27 15-5-2024
-----------------
- added FrameBuffer class
- implemented buffers.h

v0.1.26 14-5-2024
-----------------
- lots of clean-up

v0.1.25 14-5-2024
-----------------
- directional shadow mapping

v0.1.24 11-5-2024
-----------------
- quad with shadowmap working
- implemented textureType

v0.1.23 11-5-2024
-----------------
- fixed Blinn-Phong
- added convertToLinearSpace to Texture and Model functions
- added gamma correction
- implemented glCreateTextures

v0.1.22 9-5-2024
-----------------
- added Blinn-Phong

v0.1.21 21-4-2024
-----------------
- smoothstep instead of clamp for flashlight/spotlight
- added emission back for flashlight :D
- replaced light.shader with singleColor.shader (was basically same shader)
- LightCube draw changed to singleColor and ssbo
- added init for ssbo's (moved glNamedBufferStorage to top of renderloop)

v0.1.19 20-4-2024
-----------------
- added specific emissionMap,
- 3 variants to use emission for flashlight in temp.cpp
- removed emission from spotlight and flashlight
- added texture loading to AI TEXTURE ASSET MANAGER 

v0.1.18 19-4-2024
-----------------
- uniformBuffer projectionView -> projection
- multilight.shader strength toegevoegd
- multilight.shader e.e.a. aangepast qua berekeningen world/view space
- cleanup and commented multilight.shader
- normalViewCPU renamed -> NormalMatrix
- added ambient to dirLight

v0.1.17 18-4-2024
-----------------
- requires version 4.5
- alle draw calls zijn nu instanced (muv skybox en xyz)
- added ssbo's

v0.1.16 16-4-2024
-----------------
- added Geometry shader to multiLight

v0.1.15 16-4-2024
-----------------
- xyz Geometry shader (with and without Interface Blocks)

v0.1.14 13-4-2024
-----------------
- changed model texture loading
- AI TEXTURE ASSET MANAGER

v0.1.13 9-4-2024
-----------------
- removed all framebuffer code

v0.1.12 9-4-2024
-----------------
- implemented a sort of rearviewmirror with framebuffers

v0.1.11 8-4-2024
-----------------
- floor outline, with toggle

v0.1.10 6-4-2024
-----------------
- TODO's nagelopen
- addVertexAttributeLayout & addUniformBufferSubData asserts gefixed
- added SPtr & UPtr

v0.1.09 5-4-2024
-----------------
- model textures nu met shared_ptr
- model textures inladen nu met meshTextures& ipv copy
- veel opgeschoond
- wel of niet hoofdletter bij data members gelijkgetrokken

v0.1.08 5-4-2024
-----------------
- textures als raw pointers in model.h
- texture move constructors etc. gemaakt

v0.1.07 4-4-2024
-----------------
- modelView op CPU laten berekenen

v0.1.06 4-4-2024
-----------------
- Rugzak zichtbaar!

v0.1.05 24-3-2024
-----------------
- mulitlight
- before mesh implementing

v0.1.04 21-3-2024
-----------------
- error checking glGetUniformLocation
- added Global.h
- error checking everywhere

v0.1.03 17-3-2024
-----------------
End chapter Materials

v0.1.02 17-3-2024
-----------------
added fov and aspectratio to Camera class
after window resizing, perspective, fov and aspectratio is now corrected

v0.1.01 16-3-2024
-----------------
setVertexAttributeOffset handles negative new Offset and sets Stride + added asserts

v0.1 16-3-2024
---------------
base