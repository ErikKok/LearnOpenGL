TO DO
- Calling glUniform is quite expensive during run-time. Structure your programme so that glUniform is only called when the value needs to change. This might be the case every time that you draw a new object (e.g. its position might be different), but some uniforms may not change often (e.g. projection matrix).

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