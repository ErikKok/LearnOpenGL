Reminder:
- cubemaps can be problematic around their edges: if each face is mip-mapped independently some seams will be noticeable around the borders
- try to architect your renderer in a way that you can do pretty much all of your data uploads at the beginning of the frame
- samplers and other opaque shader types can be given explicit binding points https://juandiegomontoya.github.io/modern_opengl.html#sampler%20objects
- use a constant array in the shader, e.g. for fullscreen quads

TO DO
- Logging https://antongerdelan.net/opengl/glcontext2.html
- time class maken
- Optimization
	- avoid using vec3 at all in interface blocks https://stackoverflow.com/questions/47616015/opengl-and-glsl-memory-alignment-for-uniforms-and-varyings
	- Uniform location cache https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions?tab=readme-ov-file#ideal-way-of-retrieving-all-uniform-names
	- Calling glUniform is quite expensive during run-time. Structure your programme so that glUniform is only called when the value needs to change. This might be the case every time that you draw a new object (e.g. its position might be different), but some uniforms may not change often (e.g. projection matrix).
	- glResult als global opslaan ipv elke keer opvragen
	- combine buffer and bufferDataStore class, with ubo and ssbo as inheritance
	- for now don't use ubo, only ssbo
	- cost of state changes: https://community.khronos.org/t/relative-costs-of-state-changes-pipelinelayout-compatibility/108644
- Lighting
	- implement energy conservation? see comments at Blinn-Phong
	- Blinn-Phong can also produce invalid highlights for objects that are lit from behind, see comments at Blinn-Phong
	- which attenuation values/formula is best/sufficient in combination with gamma correction
	- use sampler2DShadow free 4x AA https://stackoverflow.com/questions/22419682/glsl-sampler2dshadow-and-shadow2d-clarification
	- transparency shadow testen
	- structs in 1x zetten ipv per element in sendToShader()
	- TODO Hard limit pointlights count. Of via SSBO met een variabel aantal. Of via UBO met een vast max aantal, en dan het daadwerkelijke gebruikte aantal ook doorgeven voor de loop in de Shader
	- ECS - m_id is used as an index for the setters, and will break if a light is removed from the vector, or the order changes, replace with ECS
	- Shadows:
		- https://www.reddit.com/r/opengl/comments/iy4jnp/comment/g6ihvom/
		- https://gamedev.stackexchange.com/questions/139545/how-do-modern-game-engines-handle-many-shadow-casting-lights
	- flashlight
		- does not light the underside of floor
- implement Bindless Textures
- font rendering
- TODO BiTangent coords are extracted in Model::processMesh and used for correction of TexCoords. They are also stored in the vertices and VBO, but not used. Maybe in the future?
- could tangent be replaced with * 0.0f?
- obj van cube en hexagon fixen
- ECS - move Camera into FrameBuffer? zit al in Light... moet die in de framebuffer?
- ECS - make sun static in-Class, and check for getOn in goRender loop
- ECS - implement submitRO(), but RO could change memory location afterwards as it's a raw *. needs to be removed from vector before being updated or something, and then re-added.

v0.2.87 3-7-2024
-----------------
- implemented sampler2DShadow 
- some optimizations, reversed the if outside frustum statement and moved some calucations to that if
- dirLight dirLightShadowCoord reduced from vec4 to vec3, no need to perform perspective divide, so no need for the .w component
- 245fps -> 275fps
- applied the if outside frustum statement to the whole caluclation for spotlight, and part of dirLight (no more specular lighting outside frustum)
- 275fps -> 300~315fps

v0.2.86 2-7-2024
-----------------
- merged draw() and drawModel()
- added drawAsSingleColor in RO
- TODO's nagelopen

v0.2.85 1-7-2024
-----------------
- Light now has a unique ptr Camera instead of a raw*

v0.2.84 29-6-2024
-----------------
- fixed some small TODO's
- FrameBuffer depthMap constructor now creates it's own Texture
- implemented: Getters should return by value or by const lvalue reference

v0.2.83 28-6-2024
-----------------
- removed m_gammaCorrection from Model, unused...
- added a OpenGL scope to properly delete OpenGL stuff when closing glfwWindow

v0.2.82 27-6-2024
-----------------
- moved orthographic to camera, from framebuffer
- improved debugQuad, you can now scroll through the depthMaps
- ...and through the frustum as well

v0.2.81 26-6-2024
-----------------
- renamed and reorderd SSBO enums
- combined the two SSBO enums
- upgraded enum to enum class
- added overload unary + operator to convert SSBO to the underlying type
- commented out setType() and getBindingPoint()

v0.2.80 26-6-2024
-----------------
- removed uberSSBO, everything seem to be working

v0.2.79 26-6-2024
-----------------
- added m_ssboType

v0.2.78 25-6-2024
-----------------
- Texture Move constructor enabled
- Framebuffer now moves an existing texture to itself (instead of just filling an unique_ptr)
- added a for loop to goRender() and drawOutline()
- added a std::vector<std::pair> for FBO's and Shaders

v0.2.77 25-6-2024
-----------------
- first time goRender! working

v0.2.76 22-6-2024
-----------------
- moved applyCameraOffset() to Global
- implemented drawOutline()
- moved initStencilBuffer() and clearStencilBuffer() to Renderer Class

v0.2.75 21-6-2024
-----------------
- moved cameraFlashLight out of Global
- added setCameraOffset()
- implemented: 		- flashlight camera movement is duplicated in processInput/mouse_callback/scroll_callback, could be linked/feed from player camera I guess
- fixed: 		- flashLight shines from the player camera (0,0,0), but casts shadow from this camera with flashLightShadowOffset (shadows won't be visible otherwise)
- fixed: 		- flashlight frustum is niet meer beeldvullend NA eerste gebruik zoomfunctie
- fixed: 		- flashlight positie updaten na player positie is geupdated met offset
- fixed: 		- flashlight frustum draait niet goed mee als je je omdraait, blijft altijd naast je, draait niet OM je heen, maar om z'n eigen as
- niet meer het geval: 		- flashlight camera/FBO settings zijn dubbelop / moeten syncen (getOrthographic / view / projection / fov / farnearplanes / aspectratio / nog meer?)
- implemented: sync camera and light positions
- added texture.getAspectRatio()

v0.2.74 21-6-2024
-----------------
- fixed flashLightCamera, flashLightShadowOffset is now respected in every orientation

v0.2.73 20-6-2024
-----------------
- added Camera* to light Class
- moved position spotlight[1] to top of renderloop (was lagging 1 frame)
- moved position flashlight to renderloop, from processMovement functions
- made a array of spotLightShadowCoord in Shader

v0.2.72 19-6-2024
-----------------
- quick fix shadows spotLight implemented
- clean up flashLight stuff
- clean up Shaders

v0.2.71 18-6-2024
-----------------
- migrated FlashLight to SpotLight[0] (to do: shadow)

v0.2.70 18-6-2024
-----------------
- added BufferDataStore.uploadUntilSubset()
- removed updateAndUploadSubset()
- added ability to upload within the update functions
- see section // Calculate dynamic models/transforms and SSBO's for 3 types of usage 
- removed all but 1 default parameters, they will cause confusion because of implicit conversion between int and bool
- fixed m_count in Light Class
- disabled most useShader()s in Renderer, not needed with uberSSBO (no speed increase)

v0.2.69 17-6-2024
-----------------
- uberSSBO implemented
- clean-up
- old method with 6 ssbo's commented, so you can compare
- be aware you need to fill the uberSSBO struct fully per element. not struct after struct, but element after element. just push one struct into the ssbo, not fill 1 struct, push, fill next, push, stuff won't be aligned...
- pros:
	- no need to use different bindings for different renderPassType passes, just bind the uberSSBO once
	- just need to define one SSBO (saves 5 lines of code)
	- minor speed increase from 250 to 260fps (from 6 to 1 ssbo's)
- cons
 	- need a custom struct to begin with
	- need to add uberSSBO to std::vector<std::variant and all asserts
	- the arrays within the ssbo in the shader AND in the struct itself needs a fixed size
	- need to fix RO which currently resizes the DataBufferStore with the amount of instances, with uberSSBO you just have 1 huge element/struct (temp workaround does not give any significant speed advantage)
	- if a RenderObject just needs 1 of the mat4's in the uberSSBO it is a bit wastefull (lightCube)
	- uploadUntilSubset() does not work with uberSSBO, because that is just 1 element anyway
- neutral
	- same amount of lines of code to fill the data, one for each mat4, which contains roughly the same code

v0.2.68 16-6-2024
-----------------
- added std::vector<PointLight> pointLights with m_id
- changed some logic for pointLights, using for loops

v0.2.67 15-6-2024
-----------------
- clean up both multiLight shaders
- optimized shader (mainly multiLightNormalMapping)
- changed inheritance spotlight/pointlight and added some loops for pointlight

v0.2.66 15-6-2024
-----------------
- moved TBN calculations to the vertex shader for all lights
- lost 5-8fps... (but introduced for loop in vertex, 2nd in/out interface block, etc.)

v0.2.65 15-6-2024
-----------------
- for loops nagelopen
- normalize DirLightDirection op cpu, moved uniform to vertex shader

14-6-2024 normalmapping off for cube/floor/backpack: 272fps, on 230fps. before optimization

v0.2.64 11-6-2024
-----------------
- added enum ssboTypes & enum ssboBindingPoints
- Made a class of RenderObject
- added BufferDataStore.cpp
- added type check for BufferDataStore
 
v0.2.63 10-6-2024
-----------------
- nu wel werkend...

v0.2.62 10-6-2024
-----------------
- BufferDataStore via ShaderStorageBuffer werkt nu zo lijkt het

v0.2.61 9-6-2024
-----------------
- replaced std::any with std::variant
- first try to integrate BufferDataStore into ShaderStorageBuffer, everything compiles now

v0.2.60 8-6-2024
-----------------
- cleaned up the loop in addVertexAttributeLayoutStep2
- made verexAttribute friend of vertexarray, ditched some getters
- ditched addVertexAttributeLayout() 2x
- moved VertexAttribute to VertexArray

v0.2.59 7-6-2024
-----------------
- moved bind calls VBO/EBO to mesh constructor
- drawSingleColor now uses RenderObject
- FloorOutline through RO
- moved ebo/vbo binding from renderer to mesh constructor/setupMesh()

v0.2.58 6-6-2024
-----------------
- ditched BufferSubData Class altogether
- renamed BufferSubDataLayout Class to BufferDataStore

v0.2.57 6-6-2024
-----------------
- RO lightcube werkt
- rewritten BufferSubData

v0.2.56 5-6-2024
-----------------
- test RO lightcube, werkt nu wel met vec4 niet met een vector van vec4

v0.2.55 5-6-2024
-----------------
- test RO lightcube 

v0.2.54 4-6-2024
-----------------
- BufferSubData rewritten, can now upload and create its own buffer
- accepts vec4 and mat4
- UniformBuffer constructor does not reserve data, only creates a buffer
- added bindingpoint to UniformBuffer

v0.2.53 4-6-2024
-----------------
- clean-up
- Model is now friend class of Renderer

v0.2.52 4-6-2024
-----------------
- implemented RenderObject for Model
- moved drawModel from Model to Renderer
- Mesh is now a Unique* in RO

v0.2.51 4-6-2024
-----------------
- implemented RenderObject for floor
- fixed SSBO element for renderpasses
- improved model calculation for cube

v0.2.50 3-6-2024
-----------------
- added a check, and correction, if TexCoords are oriented the right way in Model::processMesh
- added RenderObject, implemented for Cube

v0.2.44 2-6-2024
-----------------
- flashlight clean-up

v0.2.43 2-6-2024
-----------------
- flashlight werkt

v0.2.42 1-6-2024
-----------------
- shader: in useShader een if maken of de shader al actief is? (zie Shader::getLocation voor assert) (Global::shaderCurrentlyActive)
- fout uit SpotLight::updateColor gehaald
- normal mapping werkt een beetje

v0.2.41 1-6-2024
-----------------
- fout opgelost (direction werd niet geüpdatet in renderloop)
- copy van MultiLight gemaakt, unifroms gekoppeld hieraan, beide Shaders geven nu hetzelfde resultaat

v0.2.4 1-6-2024
-----------------
- normal mapping niet gelukt
- fout in belichting dirlight sinds light class, nu terug naar oude situatie met alleen multilight shader
- let op de if in Renderer::drawModel regel #181

v0.2.3 1-6-2024
-----------------
- start implementing normal mapping dirlight

v0.2.2 29-5-2024
-----------------
- removed s_depthMapPassActive
- works with 2 saperate shaders during normal render pass
- added useShader() to light functions
- enabled textureType::normal in Model::processMesh, Renderer::drawModel and Material

v0.2.1 28-5-2024
-----------------
- moved mesh::draw to Renderer

v0.2.0 28-5-2024
-----------------
- refactored to use DSA
- replaced VAO/VBO/EBO with Mesh
- clean-up

v0.1.44 27-5-2024
-----------------
- UBO converted to DSA
- use create instead of gen and bind
- converted everything to immutable
- Renderer now accepts a Mesh

v0.1.43 27-5-2024
-----------------
- VAO/VBO/EBO converted to DSA

v0.1.42 27-5-2024
-----------------
- v4.5 -> v4.6
- Texture: converted to DSA, and changed convertToLinearSpace logic

v0.1.41 26-5-2024
-----------------
- Model, VertexAttribute, renderer .cpp gemaakt/bijgewerkt
- check of functies/return types const kunnen worden
- check of functies wel refs innemen ipv copy's
- removed all glDraw's from main
- namen depthMap en shadowMap consistent maken

v0.1.40 25-5-2024
-----------------
- replaced cube with cubeStrip and a cube with less elements
- sorted Data.cpp

v0.1.39 25-5-2024
-----------------
- replaced new with std::make_unique in mesh.cpp
- added dirLightShader to Renderer class
- made arguments mesh/model Draw() const
- added drawSingleColor()
- replaced most glDraws with Draw()
- moved XYZ to drawXYZ(), and used singleColorShader instead of xyzShader

v0.1.38 22-5-2024
-----------------
- added Renderer Class
- added Material Struct

v0.1.37 22-5-2024
-----------------
- moved (almost?) everything to Camera class
- removed Global::view and ::projection
- Camera now recalulates view/projectionmatrix if anything relevant changes
- added OrthographicCamera

v0.1.36 22-5-2024
-----------------
- moved framebuffer/depthmap further to camera class

v0.1.35 22-5-2024
-----------------
- added camera's for the shadowmaps
- moved framebuffer/depthmap partly to camera class

v0.1.34 21-5-2024
-----------------
- added ability to visualize frustum

v0.1.33 19-5-2024
-----------------
- moved flashlight to new class
- adjusted flastlight toggle (no more shader* in Global)

v0.1.32 18-5-2024
-----------------
- moved dir/spotlight to new class

v0.1.32 18-5-2024
-----------------
- added light class
- added flashLight shadow

v0.1.31 17-5-2024
-----------------
- depthPass logic added to FrameBuffer class

v0.1.30 16-5-2024
-----------------
- fixed casting shadows outside depthMap and light's frustum
- depthMap through materials
- added PCF

v0.1.29 16-5-2024
-----------------
- replaced and move ubo/uniform calculations to cpu in xyz en skybox shader

v0.1.28 15-5-2024
-----------------
- made debugQuad toggable
- added ShaderStorageBuffer class

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