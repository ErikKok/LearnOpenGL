Simple 3D Engine / Renderer
===========================

Started while learning C++ and OpenGL and slowly implementing more and more features.

Features:
- OpenGL 4.6, Fully DSA (Direct State Access)
- Variable FPS
- Fixed Rate Physics engine with extrapolation
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

Controls:
WASD = Movement
<br>Z = Down
<br>X = Up
<br>
<br>F = Flashlight
<br>K = Show Frustum
<br>L = PolygonMode (line/point/fill)
<br>M = Release mouse
<br>O = Draw outline
<br>P = Pause
<br>Q = Show/toggle shadowmap
<br>V = Toggle v-sync
<br>
<br>SHIFT = Run
<br>ESC = Quit

Dependencies:
- ASSIMP v5.2.5
- glad 0.1.36
- GLFW 3.4
- ImGui v1.91.3
- stb_image - v2.29