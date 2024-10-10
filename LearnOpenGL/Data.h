#pragma once

#include <glm/glm.hpp>

#include "array"
#include "string"
#include "vector"

namespace Data{

    const std::vector<std::string> skybox1Faces{
       "Textures\\skybox1\\right.jpg",
       "Textures\\skybox1\\left.jpg",
       "Textures\\skybox1\\top.jpg",
       "Textures\\skybox1\\bottom.jpg",
       "Textures\\skybox1\\front.jpg",
       "Textures\\skybox1\\back.jpg",
    };

    const std::vector quad{ // NDC fullscreen quad (2 triangles)
    //   X      Y     Unused  TextureX  TextureY   Unused
        -1.0f,  1.0f, 0.0f,   0.0f,     1.0f,      0.0f,     0.0f,     0.0f,
        -1.0f, -1.0f, 0.0f,   0.0f,     0.0f,      0.0f,     0.0f,     0.0f,
         1.0f, -1.0f, 0.0f,   1.0f,     0.0f,      0.0f,     0.0f,     0.0f,
         1.0f,  1.0f, 0.0f,   1.0f,     1.0f,      0.0f,     0.0f,     0.0f,
    };

    const std::vector quadIndices{   
        0u, 1u, 2u,
        0u, 2u, 3u,
    };

    //VertexArray cubeCoordsOnlyVAO; // 288 + 144 = 432 bytes
    //VertexBuffer cubeCoordsOnlyVBO(sizeof(Data::cubeCoordsOnly), &Data::cubeCoordsOnly);
    //VertexAttributeLayout cubeCoordsOnlyLayout;
    //cubeCoordsOnlyLayout.pushVertexAttributeLayout<float>(3);
    ////cubeCoordsOnlyLayout.setVertexStride(32);
    //cubeCoordsOnlyVAO.addVertexAttributeLayout(cubeCoordsOnlyVBO, cubeCoordsOnlyLayout);
    //cubeEBO.bindElementBuffer();

    constexpr std::array cubeCoordsOnly{
    //   X      Y      Z
        // Back face          
        -1.0f, -1.0f, -1.0f,    // 0 Bottom-left
         1.0f,  1.0f, -1.0f,    // 1 top-right
         1.0f, -1.0f, -1.0f,    // 2 bottom-right
        -1.0f,  1.0f, -1.0f,    // 3 top-left
        // Front face           
        -1.0f, -1.0f,  1.0f,    // 4 bottom-left
         1.0f, -1.0f,  1.0f,    // 5 bottom-right
         1.0f,  1.0f,  1.0f,    // 6 top-right
        -1.0f,  1.0f,  1.0f,    // 7 top-left
        // Left face            
        -1.0f,  1.0f,  1.0f,    // 8 top-right
        -1.0f,  1.0f, -1.0f,    // 9 top-left
        -1.0f, -1.0f, -1.0f,    // 10 bottom-left
        -1.0f, -1.0f,  1.0f,    // 11 bottom-right
        // Right face           
         1.0f,  1.0f,  1.0f,    // 12 top-left
         1.0f, -1.0f, -1.0f,    // 13 bottom-right
         1.0f,  1.0f, -1.0f,    // 14 top-right   
         1.0f, -1.0f,  1.0f,    // 15 bottom-left 
        // Bottom face         
        -1.0f, -1.0f, -1.0f,    // 16 top-right
         1.0f, -1.0f, -1.0f,    // 17 top-left
         1.0f, -1.0f,  1.0f,    // 18 bottom-left
        -1.0f, -1.0f,  1.0f,    // 19 bottom-right
        // Top face             
        -1.0f,  1.0f, -1.0f,    // 20 top-left
         1.0f,  1.0f,  1.0f,    // 21 bottom-right
         1.0f,  1.0f, -1.0f,    // 22 top-right   
        -1.0f,  1.0f,  1.0f,    // 23 bottom-left 
    };

    const std::vector cube{
    //   X      Y      Z       TextureX  TextureY  Normal1  Normal2  Normal3
        // Back face                                                          
        -1.0f, -1.0f, -1.0f,   0.0f,     0.0f,     0.0f,    0.0f,    -1.0f,   // 0 Bottom-left
         1.0f,  1.0f, -1.0f,   1.0f,     1.0f,     0.0f,    0.0f,    -1.0f,   // 1 top-right
         1.0f, -1.0f, -1.0f,   1.0f,     0.0f,     0.0f,    0.0f,    -1.0f,   // 2 bottom-right
        -1.0f,  1.0f, -1.0f,   0.0f,     1.0f,     0.0f,    0.0f,    -1.0f,   // 3 top-left
        // Front face                                                          
        -1.0f, -1.0f,  1.0f,   0.0f,     0.0f,     0.0f,    0.0f,     1.0f,   // 4 bottom-left
         1.0f, -1.0f,  1.0f,   1.0f,     0.0f,     0.0f,    0.0f,     1.0f,   // 5 bottom-right
         1.0f,  1.0f,  1.0f,   1.0f,     1.0f,     0.0f,    0.0f,     1.0f,   // 6 top-right
        -1.0f,  1.0f,  1.0f,   0.0f,     1.0f,     0.0f,    0.0f,     1.0f,   // 7 top-left
        // Left face                                                           
        -1.0f,  1.0f,  1.0f,   1.0f,     0.0f,    -1.0f,    0.0f,     0.0f,   // 8 top-right
        -1.0f,  1.0f, -1.0f,   1.0f,     1.0f,    -1.0f,    0.0f,     0.0f,   // 9 top-left
        -1.0f, -1.0f, -1.0f,   0.0f,     1.0f,    -1.0f,    0.0f,     0.0f,   // 10 bottom-left
        -1.0f, -1.0f,  1.0f,   0.0f,     0.0f,    -1.0f,    0.0f,     0.0f,   // 11 bottom-right
        // Right face                                                          
         1.0f,  1.0f,  1.0f,   1.0f,     0.0f,     1.0f,    0.0f,     0.0f,   // 12 top-left
         1.0f, -1.0f, -1.0f,   0.0f,     1.0f,     1.0f,    0.0f,     0.0f,   // 13 bottom-right
         1.0f,  1.0f, -1.0f,   1.0f,     1.0f,     1.0f,    0.0f,     0.0f,   // 14 top-right   
         1.0f, -1.0f,  1.0f,   0.0f,     0.0f,     1.0f,    0.0f,     0.0f,   // 15 bottom-left 
        // Bottom face                                                        
        -1.0f, -1.0f, -1.0f,   0.0f,     1.0f,     0.0f,   -1.0f,     0.0f,   // 16 top-right
         1.0f, -1.0f, -1.0f,   1.0f,     1.0f,     0.0f,   -1.0f,     0.0f,   // 17 top-left
         1.0f, -1.0f,  1.0f,   1.0f,     0.0f,     0.0f,   -1.0f,     0.0f,   // 18 bottom-left
        -1.0f, -1.0f,  1.0f,   0.0f,     0.0f,     0.0f,   -1.0f,     0.0f,   // 19 bottom-right
        // Top face                                                            
        -1.0f,  1.0f, -1.0f,   0.0f,     1.0f,     0.0f,    1.0f,     0.0f,   // 20 top-left
         1.0f,  1.0f,  1.0f,   1.0f,     0.0f,     0.0f,    1.0f,     0.0f,   // 21 bottom-right
         1.0f,  1.0f, -1.0f,   1.0f,     1.0f,     0.0f,    1.0f,     0.0f,   // 22 top-right   
        -1.0f,  1.0f,  1.0f,   0.0f,     0.0f,     0.0f,    1.0f,     0.0f,   // 23 bottom-left 
    };

    const std::vector cubeIndices{
        // Z+ back      
        0u, 1u, 2u,
        1u, 0u, 3u,
        // Z- front
        4u, 5u, 6u,
        6u, 7u, 4u,
        // X+ left
        8u, 9u, 10u,
        10u, 11u, 8u,
        // X- right
        12u, 13u, 14u,
        13u, 12u, 15u,
        // Y- bottom
        16u, 17u, 18u,
        18u, 19u, 16u,
        // Y+ top
        20u, 21u, 22u,
        21u, 20u, 23u,
    };

    // This strip does NOT work with Textures or Normals
    //constexpr std::array cubeStrip{ // GL_TRIANGLE_STRIP
    //    //   X       Y       Z       TextureX  TextureY  Normal1  Normal2   Normal3
    //     1.0f,	 1.0f,	 1.0f,   1.0f,     1.0f,     0.0f,    0.0f,     1.0f,    // 0 Front-top-right
    //    -1.0f,	 1.0f,	 1.0f,   0.0f,     1.0f,     0.0f,    0.0f,     1.0f,    // 1 Front-top-left
    //     1.0f,	 1.0f,	-1.0f,   1.0f,     1.0f,     0.0f,    0.0f,    -1.0f,    // 2 Back-top-right
    //    -1.0f,   1.0f,	-1.0f,   0.0f,     1.0f,     0.0f,    0.0f,    -1.0f,    // 3 Back-top-left
    //     1.0f,	-1.0f,	 1.0f,   1.0f,     0.0f,     0.0f,    0.0f,     1.0f,    // 4 Front-bottom-right
    //    -1.0f,	-1.0f, 	 1.0f,   0.0f,     0.0f,     0.0f,    0.0f,     1.0f,    // 5 Front-bottom-left
    //    -1.0f,	-1.0f,	-1.0f,   0.0f,     0.0f,     0.0f,    0.0f,    -1.0f,    // 6 Back-bottom-left
    //     1.0f,	-1.0f,	-1.0f,   1.0f,     0.0f,     0.0f,    0.0f,    -1.0f,    // 7 Back-bottom-right
    //};

    //VertexArray cubeStripCoordsOnlyVAO; // 96 + 56 = 152 bytes
    //VertexBuffer cubeStripCoordsOnlyVBO(sizeof(Data::cubeStripCoordsOnly), &Data::cubeStripCoordsOnly);
    //VertexAttributeLayout cubeStripCoordsOnlyLayout;
    //cubeStripCoordsOnlyLayout.pushVertexAttributeLayout<float>(3);
    //cubeStripCoordsOnlyVAO.addVertexAttributeLayout(cubeStripCoordsOnlyVBO, cubeStripCoordsOnlyLayout);
    //ElementBuffer cubeStripEBO(sizeof(Data::cubeStripIndices), &Data::cubeStripIndices);

    constexpr std::array cubeStripCoordsOnly{
         1.0f,	 1.0f,	 1.0f,      // 0 Front-top-right
        -1.0f,	 1.0f,	 1.0f,      // 1 Front-top-left
         1.0f,	 1.0f,	-1.0f,      // 2 Back-top-right
        -1.0f,   1.0f,	-1.0f,      // 3 Back-top-left
         1.0f,	-1.0f,	 1.0f,      // 4 Front-bottom-right
        -1.0f,	-1.0f, 	 1.0f,      // 5 Front-bottom-left
        -1.0f,	-1.0f,	-1.0f,      // 6 Back-bottom-left
         1.0f,	-1.0f,	-1.0f,      // 7 Back-bottom-right
    };

    constexpr std::array cubeStripIndices{
        3, 2, 6, 7, 4, 2, 0,
        3, 1, 6, 5, 4, 1, 0
    };

    //// All vertexes are defined, redundant data
    //constexpr std::array cube2{
    //// X     Y      Z       Texture1     Texture2       Normal1  Normal2   Normal3
    //// Back face
    // -0.5f, -0.5f, -0.5f,   0.0f,        0.0f,          0.0f,    0.0f,     -1.0f,     // Bottom-left
    //  0.5f,  0.5f, -0.5f,   1.0f,        1.0f,          0.0f,    0.0f,     -1.0f,     // top-right
    //  0.5f, -0.5f, -0.5f,   1.0f,        0.0f,          0.0f,    0.0f,     -1.0f,     // bottom-right
    //  0.5f,  0.5f, -0.5f,   1.0f,        1.0f,          0.0f,    0.0f,     -1.0f,     // top-right
    // -0.5f, -0.5f, -0.5f,   0.0f,        0.0f,          0.0f,    0.0f,     -1.0f,     // bottom-left
    // -0.5f,  0.5f, -0.5f,   0.0f,        1.0f,          0.0f,    0.0f,     -1.0f,     // top-left
    // // Front face                                                                    
    // -0.5f, -0.5f,  0.5f,   0.0f,        0.0f,          0.0f,    0.0f,      1.0f,     // bottom-left
    //  0.5f, -0.5f,  0.5f,   1.0f,        0.0f,          0.0f,    0.0f,      1.0f,     // bottom-right
    //  0.5f,  0.5f,  0.5f,   1.0f,        1.0f,          0.0f,    0.0f,      1.0f,     // top-right
    //  0.5f,  0.5f,  0.5f,   1.0f,        1.0f,          0.0f,    0.0f,      1.0f,     // top-right
    // -0.5f,  0.5f,  0.5f,   0.0f,        1.0f,          0.0f,    0.0f,      1.0f,     // top-left
    // -0.5f, -0.5f,  0.5f,   0.0f,        0.0f,          0.0f,    0.0f,      1.0f,     // bottom-left
    // // Left face                                                                     
    // -0.5f,  0.5f,  0.5f,   1.0f,        0.0f,         -1.0f,    0.0f,      0.0f,     // top-right
    // -0.5f,  0.5f, -0.5f,   1.0f,        1.0f,         -1.0f,    0.0f,      0.0f,     // top-left
    // -0.5f, -0.5f, -0.5f,   0.0f,        1.0f,         -1.0f,    0.0f,      0.0f,     // bottom-left
    // -0.5f, -0.5f, -0.5f,   0.0f,        1.0f,         -1.0f,    0.0f,      0.0f,     // bottom-left
    // -0.5f, -0.5f,  0.5f,   0.0f,        0.0f,         -1.0f,    0.0f,      0.0f,     // bottom-right
    // -0.5f,  0.5f,  0.5f,   1.0f,        0.0f,         -1.0f,    0.0f,      0.0f,     // top-right
    // // Right face                                                                    
    //  0.5f,  0.5f,  0.5f,   1.0f,        0.0f,          1.0f,    0.0f,      0.0f,     // top-left
    //  0.5f, -0.5f, -0.5f,   0.0f,        1.0f,          1.0f,    0.0f,      0.0f,     // bottom-right
    //  0.5f,  0.5f, -0.5f,   1.0f,        1.0f,          1.0f,    0.0f,      0.0f,     // top-right   
    //  0.5f, -0.5f, -0.5f,   0.0f,        1.0f,          1.0f,    0.0f,      0.0f,     // bottom-right
    //  0.5f,  0.5f,  0.5f,   1.0f,        0.0f,          1.0f,    0.0f,      0.0f,     // top-left
    //  0.5f, -0.5f,  0.5f,   0.0f,        0.0f,          1.0f,    0.0f,      0.0f,     // bottom-left 
    // // Bottom face                                                                  
    // -0.5f, -0.5f, -0.5f,   0.0f,        1.0f,          0.0f,   -1.0f,      0.0f,     // top-right
    //  0.5f, -0.5f, -0.5f,   1.0f,        1.0f,          0.0f,   -1.0f,      0.0f,     // top-left
    //  0.5f, -0.5f,  0.5f,   1.0f,        0.0f,          0.0f,   -1.0f,      0.0f,     // bottom-left
    //  0.5f, -0.5f,  0.5f,   1.0f,        0.0f,          0.0f,   -1.0f,      0.0f,     // bottom-left
    // -0.5f, -0.5f,  0.5f,   0.0f,        0.0f,          0.0f,   -1.0f,      0.0f,     // bottom-right
    // -0.5f, -0.5f, -0.5f,   0.0f,        1.0f,          0.0f,   -1.0f,      0.0f,     // top-right
    // // Top face                                                                     
    // -0.5f,  0.5f, -0.5f,   0.0f,        1.0f,          0.0f,    1.0f,      0.0f,     // top-left
    //  0.5f,  0.5f,  0.5f,   1.0f,        0.0f,          0.0f,    1.0f,      0.0f,     // bottom-right
    //  0.5f,  0.5f, -0.5f,   1.0f,        1.0f,          0.0f,    1.0f,      0.0f,     // top-right   
    //  0.5f,  0.5f,  0.5f,   1.0f,        0.0f,          0.0f,    1.0f,      0.0f,     // bottom-right
    // -0.5f,  0.5f, -0.5f,   0.0f,        1.0f,          0.0f,    1.0f,      0.0f,     // top-left
    // -0.5f,  0.5f,  0.5f,   0.0f,        0.0f,          0.0f,    1.0f,      0.0f,     // bottom-left 
    //};

    //constexpr std::array cube2Indices{
    //     0u, 1u, 2u,     // Z+ back
    //     3u, 4u, 5u,
    //     6u, 7u, 8u,     // Z- front
    //     9u, 10u, 11u,
    //     12u, 13u, 14u,  // X+ left
    //     15u, 16u, 17u,
    //     18u, 19u, 20u,  // X- right
    //     21u, 22u, 23u,
    //     24u, 25u, 26u,  // Y- bottom
    //     27u, 28u, 29u,
    //     30u, 31u, 32u,  // Y+ top
    //     33u, 34u, 35u,
    //};

    constexpr glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.5f,  0.0f),

        glm::vec3(0.0f,  -2.5f,  0.0f),
        glm::vec3(2.0f,  0.5f,  0.0f), //
        glm::vec3(0.0f,  0.0f,  0.0f), // wall

        glm::vec3(0.0f,  2.0f,  0.0f),
        glm::vec3(0.0f,  4.0f,  0.0f), //
        glm::vec3(0.0f,  6.0f,  0.0f),

        glm::vec3(0.0f,  8.5f,  0.0f),
        glm::vec3(0.0f,  0.5f,  2.0f), //
        glm::vec3(0.0f,  0.5f,  4.0f),
    };

    const std::vector floor{ // Hexagon
          // Positions          // Texture    // Normal         
          0.0f,   0.0f,  0.0f,  0.5f, 0.5f,   0.0f, 0.0f, -1.0f,
          0.0f,   1.0f,  0.0f,  0.5f, 1.0f,   0.0f, 0.0f, -1.0f,
          0.67f,  0.50f, 0.0f,  1.0f, 0.75f,  0.0f, 0.0f, -1.0f,
          0.67f, -0.50f, 0.0f,  1.0f, 0.25f,  0.0f, 0.0f, -1.0f,
          0.0f,  -1.0f,  0.0f,  0.5f, 0.0f,   0.0f, 0.0f, -1.0f,
         -0.67f, -0.50f, 0.0f,  0.0f, 0.25f,  0.0f, 0.0f, -1.0f,
         -0.67f,  0.50f, 0.0f,  0.0f, 0.75f,  0.0f, 0.0f, -1.0f,
    };

    const std::vector floorIndices{
         0u, 1u, 2u,
         0u, 2u, 3u,
         0u, 3u, 4u,
         0u, 4u, 5u,
         0u, 5u, 6u,
         0u, 6u, 1u,
    };

    //constexpr glm::mat4 biasMatrix(
    //    0.5f, 0.0f, 0.0f, 0.0f,
    //    0.0f, 0.5f, 0.0f, 0.0f,
    //    0.0f, 0.0f, 0.5f, 0.0f,
    //    0.5f, 0.5f, 0.5f, 1.0f
    //);

    //So your depthBias matrix, which you use to convert from normalized device coordinates(in ranage[-1, 1]) to texture coordinates(in range[0, 1]), should look like this:

    //constexpr glm::mat4 biasMatrix(
    //    0.5, 0.0, 0.0, 0.0,
    //    0.0, 0.5, 0.0, 0.0,
    //    0.0, 0.0, 0.5, 0.0,
    //    0.5, 0.5, 0.5, 1.0);
    //or this:

    //constexpr glm::mat4 biasMatrix(
    //    glm::vec4(0.5, 0.0, 0.0, 0.0),
    //    glm::vec4(0.0, 0.5, 0.0, 0.0),
    //    glm::vec4(0.0, 0.0, 0.5, 0.0),
    //    glm::vec4(0.5, 0.5, 0.5, 1.0));
}