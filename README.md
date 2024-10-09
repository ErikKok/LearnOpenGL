Simple 3D Engine / Renderer

Started while learning OpenGL and slowly implementing more features

Features:
- OpenGL 4.6, Fully DSA (Direct State Access)
- Fixed Physics Speed with extrapolation (independent of Variable FPS)
- (Almost) everything is abstracted into Classes
- Every draw call is through glDrawElementsInstanced()
- ASSIMP model loader
- Normal Mapping
- Frustrums can be visualised
- Shadow Depth Map for:
  -   directional light
  -   spot light
  -   flash light 
- Shader Storage Buffer
- Materials
- RenderObjects

WASD = Movement

<br />Z = Down

<br />X = Up

F = Flashlight

K = Show Frustum

L = PolygonMode (line/point/fill)

M = Release mouse

O = Draw outline

P = Pause

Q = Show/toggle shadowmap

V = Toggle v-sync

SHIFT = Run

ESC = Quit
