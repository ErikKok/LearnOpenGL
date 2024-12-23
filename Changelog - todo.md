Reminder:
- cubemaps can be problematic around their edges: if each face is mip-mapped independently some seams will be noticeable around the borders
- try to architect your renderer in a way that you can do pretty much all of your data uploads at the beginning of the frame
- samplers and other opaque shader types can be given explicit binding points https://juandiegomontoya.github.io/modern_opengl.html#sampler%20objects
- use a constant array in the shader, e.g. for fullscreen quads

Known bugs:
- drawDebugQuad gives Undefined Behaviour warnings, because quadTexture is actually a sampler2DShadow, but used as a sampler2D (see https://community.khronos.org/t/read-depth-buffer-value-from-sampler2dshadow/53156)
- flashlight cutOffSpecular bug (since v0.2.88 (sort of mitigated in v.0.3.01))

TO DO
- Logging https://antongerdelan.net/opengl/glcontext2.html
- time class maken
- Optimization
	- avoid using vec3 at all in interface blocks https://stackoverflow.com/questions/47616015/opengl-and-glsl-memory-alignment-for-uniforms-and-varyings
	- Uniform location cache https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions?tab=readme-ov-file#ideal-way-of-retrieving-all-uniform-names
	- Calling glUniform is quite expensive during run-time. Structure your programme so that glUniform is only called when the value needs to change. This might be the case every time that you draw a new object (e.g. its position might be different), but some uniforms may not change often (e.g. projection matrix).
	- combine buffer and bufferDataStore class, with ubo and ssbo as inheritance
	- for now don't use ubo, only ssbo
	- cost of state changes: https://community.khronos.org/t/relative-costs-of-state-changes-pipelinelayout-compatibility/108644
- Lighting
	- implement energy conservation? see comments at Blinn-Phong
	- Blinn-Phong can also produce invalid highlights for objects that are lit from behind, see comments at Blinn-Phong
	- which attenuation values/formula is best/sufficient in combination with gamma correction
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
	- aspectRatio zit in Camera maar wordt gezet 'door' of adhv Texture, als depthmap Texture wijzigt moet aspectration in de Camera ook gewijzigd worden
- ECS - make sun static in-Class, and check for getOn in goRender loop
- ECS - implement submitRO(), but RO could change memory location afterwards as it's a raw *. needs to be removed from vector before being updated or something, and then re-added
- Physics / movement:

	- Apply half gravity before, and half after, position update (https://www.jwchong.com/hl/movement.html#gravity) ? (removed possible solution in 3.40 in calculateSpeed())
	- physicsFrameTime independent friction: https://stackoverflow.com/questions/43960217/framerate-independent-acceleration-decceleration
- get ImGui to show doPhysics related values in a seperate window
- ImGui vars should be (mostly) const

v0.3.60 14-11-2024
------------------
- clean up
- renamed m_maxJumpSpeed to m_maxYSpeed
- renamed calculateValues() to initValues(), and used that to initialize at start of main()
- removed limitAcceleration()
- enabled doExtrapolationStep()
- moved m_acceleration.y into resetAcceleration() and only call it in doPhysics loop
- improve acceleration accuracy according to: https://gamedev.stackexchange.com/questions/15708/how-can-i-implement-gravity -> mostly implemented already, "You can fix most of the issues with" is implemented

v0.3.59 14-11-2024
------------------
- back to basics...
- removed * Engine::physicsFrameTime from initMovement() and m_WalkAcceleration values
- removed * Engine::physicsFrameTime from calculateSpeed()
- added if (m_speed.z > -0.01f && m_speed.z < 0.01f) to limitSpeed()
- cleaned up ImGui

v0.3.58 06-11-2024
------------------
- m_gravityBoost is now accounted for by Engine::physicsFrameTime
- added updatePosition()
- switched order resetAcceleration() & handleJump()
- moved/enabled resetAcceleration() in- and outside doPhysics loop
- only reset Y-acceleration in handleJump()
- cleanup
- added framerate independent friction/aerodrag
- jumpingheight is now off...

v0.3.57 25-10-2024
------------------
- !!! do not revert: Player::handleJump() added m_speedLastFrame.y = 0.0f; // force the average speed to get to be exactly 0.0f, otherwise player will have residual speed and e.g. sink a little in the floor after landing
- be aware that extrapolation does not work right with low physicsrates, is this correct?
- tried: add * Engine::physicsFrameTime to all acceleration values and m_dryFriction/m_aeroDrag/m_gravityBoost in player.h + in initMovement() -> did *NOT* fully succeed
- friction has to be modified because of different speeds
- same with gravityBoost
- removed * Engine::physicsFrameTime from calculateSpeed()
- w+S etc. does work, just too slow
- !!! do not revert: changed layout if statement calculateViewMatrix()

v0.3.56 24-10-2024
------------------
- disabled limitAcceleration(), redundant with limitSpeed()?
- disabled resetAcceleration()
- resetAcceleration Y each physic tick: moved to handleJump(), x and Y once after doPhysics while loop. quick&dirty implementation of https://gamedev.stackexchange.com/questions/174202/how-to-handle-player-input-with-fixed-rate-variable-fps-time-step
- disabled PlayerMovement::leftright and fowardbackward in initMovement(), friction/aerodrag and opposing forces in initMovement() handle this now
- removed m_airborneDecelerationFactor
- changed m_acceleration.y += into m_acceleration.y = in initMovement() for everything
- added else ifs in Engine::processInput() to catch simultaneous keypresses
- disabled bool m_isRunning
- disabled m_maxAcceleration floats 3x
- Player::limitSpeed() tried to limit forward/rightspeed... now limits m_speed
- window starts maximized

v0.3.55 23-10-2024
------------------
- tried to get jumping right (not slowing down when turning, but also not being able to go (too much) backwards), did not succeed
- upSpeed is not correct, deleted
- added m__direction (which also works)
- Player::limitSpeed() changed according to limitAcceleration()
- clean-up
- limitSpeed(): changed snapping to zero from 0.001f to 0.01f
- lots of little changes

v0.3.54 22-10-2024
------------------
- seems like diagonal movement is now correctly limited
- implemented forward/right/upSpeed in Player::limitSpeed()
- added PlayerMovement::forward/backward to if (m_isAirborne) in Player::initMovement()
- added [[likely]] & [[unlikely]] to Player::initMovement()

v0.3.53 22-10-2024
------------------
- tweaked jumping
- added aerodrag
- added gravityBoost

v0.3.52 21-10-2024
------------------
- flashlight does not shine anymore since v0.3.5 :( -> fix: calculate m_viewMatrix in the constructor body instead of initializer list (i guess bug was because flashlight m_front wasn't calculated yet with updateCameraVectors(), and will never be recalculated, something like that)
- added m_maxWalkAcceleration, m_maxStrafeAcceleration, m_maxJumpSidewaysAcceleration
- cleanup / optimization Player::initMovement()
- moved player.m_correction into the functions
- added/changed "Both axis hits x simultaneously" in Player::limitSpeed() and Player::initMovement()

v0.3.51 21-10-2024
------------------
- player and camera on the stack
- Collision test visible in ImGui
- Collision test first implementation works!
- some ImGui layout stuff

v0.3.5 20-10-2024
-----------------
- a LOT of header clean-up!
- GE::camera -> G::camera*
- GE::player -> G::player*
- removed globalEntities.h
- moved calculateViewMatrix() to initializer list of the Camera constructor (could not use calculateViewMatrix() anymore because of if statement (spotLights does not exist yet))
- removed some print debugging
- removed buffers.h
- removed ImGui.h

v0.3.4 20-10-2024
-----------------
- got ImGui working without HesGui stuff

v0.3.3 19-10-2024
-----------------
- configure glad for use with imgui
- pressing M does not invoke movement anymore, so you can control ImGui:
	- rewritten Engine::mouse_callback(), added firstMouse, moved lastXPos/lastYPos to Global
	- changes to cursor_enter_callback() and key_callback()
	- renamed windowsHasMouseFocus to isMouseCursorEnabled
- added some stupid HesGui class stuff as workaround

v0.3.2.1 17-10-2024
-------------------
- removed old assimp / stb files

v0.3.2 17-10-2024
-----------------
- added ImGui
- stb image loader v2.29 -> v2.30
- ASSIMP v5.2.5 -> v5.4.3

v0.3.11.2 17-10-2024
--------------------
- *** movement system looks quite allright now (again)
- more cleanup
- renamed handleMovement() to calculateSpeed()
- moved setPosition from handleMovement() to doPhysics()
- renamed CameraMovement to PlayerMovement + de-capitalized + moved to Player
- de-capitalized enum class ShaderType

v0.3.11.1 17-10-2024
--------------------
- cleanup

v0.3.11 17-10-2024
------------------
- added calculateForwardSpeed()
- 0.01f precision is not enough with low framerates (30fps) in isEqual(), changed to 0.001f -> too MUCH precision -> 0.05f seems ok with 30fps
- fixed UPDOWN while not jumping floats you down slowly
	
v0.3.10 16-10-2024
------------------
- added backwards jumping
- added slowing down while airborne, but not gaining negative speed
- added returns in Engine::key_callback() and Player::initMovement()

v0.3.09 15-10-2024
------------------
- fixed movement bug introduced in 3.07 (initMovement() m_acceleration.x = -> m_acceleration.x +=)

v0.3.08 15-10-2024
------------------
- cleanup

v0.3.07 15-10-2024
------------------
- player.handleJumpextrapolation() -> Engine.doExtrapolationStep(), all three axis now
- isExtrapolationStep was altijd true -> fixed
- integrated initJump() into initMovement()

v0.3.06 14-10-2024
------------------
- tried to optimize and change some things, not 100% succeeded... i'll just leave it here for now

v0.3.05 14-10-2024
------------------
- added limitSpeed()
- integrated handleJump() into handleMovement()
- added dryFriction, drag -> aeroDrag, some maxJumpAcceleration, some cleanup and optimizations

v0.3.04 12-10-2024
------------------
- Movement fully(?) moved over to acceleration based system, seems quite allright now
- Camera::processKeyboard() no longer in use
- added ability to press both opposite directions together (and still go in a third direction)

v0.3.03 12-10-2024
------------------
- acceleration float -> vec3
- added forward and back to new movement model

v0.3.02 10-10-2024
------------------
- added follow()
- added isEqual()
- added AABB Collision test

v0.3.02 10-10-2024
------------------
- replaced useextrapolationResultPositionY with if (this == &GE::camera) in calculateViewMatrix()
- extrapolationResultPosition float -> vec3
- float cutOffSpecular = 0.01f instead of 0.1f -> bug v.0.2.88 (also in multiLightNormalMapping.shader)

v0.3.01 9-10-2024
-----------------
- added vSync toggle
- added UP - X
- float cutOffSpecular = 0.01f instead of 0.1f -> bug v.0.2.88
- cleanup
- Outline is framerate independent now

v0.3 8-10-2024
-----------------
- added Constant Physics Speed independent of Variable FPS, at least a first very rough version just for jumping

v0.2.95 7-10-2024
-----------------
- added GlobalEntities.h
- moved Player and Camera to GlobalEntities.h
- added Engine.h
- moved some GLFW control stuff from Global.h to Engine.h
- moved init() to main
- moved jumping stuff to player Class

v0.2.93 7-10-2024
-----------------
- added Player class
- renamed Global to G

v0.2.92 6-10-2024
-----------------
- added running and setMovementSpeed()
- added gravity and jumping and all that stuff that comes with it...

v0.2.91 5-10-2024
-----------------
- added setYawPitch()
- added a rudimental teleporter

v0.2.90 6-7-2024
-----------------
- depencies moved

v0.2.89 4-7-2024
-----------------
- drawDebugQuad gives Undefined Behaviour warnings, see 'Known bugs'
- drawDebugQuad now corrects for aspect ratio of the depthmap, and keeps margins fixed independent of viewport size

v0.2.88 4-7-2024
-----------------
- bug in reflections found (introduced in v0.2.67), fixed: Normalized FragPosTangent and FragPosView for specular
- added a limit to calculate specular reflections for all light types
- added a limit to calculate specular attenuation for all spot/pointLight types
- 333~347fps now

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
- fout opgelost (direction werd niet ge�pdatet in renderloop)
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