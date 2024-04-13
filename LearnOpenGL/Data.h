#pragma once

#include <glm/glm.hpp>

#include "array"
#include "string"
#include "vector"

namespace Data{

    constexpr std::array skybox1 {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    inline std::vector<std::string> skybox1Faces{
       "Textures\\skybox1\\right.jpg",
       "Textures\\skybox1\\left.jpg",
       "Textures\\skybox1\\top.jpg",
       "Textures\\skybox1\\bottom.jpg",
       "Textures\\skybox1\\front.jpg",
       "Textures\\skybox1\\back.jpg",
    };

    constexpr std::array framebuffer{ // fullscreen quad (2 triangles)
        // X     Y     Texture1     Texture2
        -1.0f,  1.0f,  0.0f,         1.0f,
        -1.0f, -1.0f,  0.0f,         0.0f,
         1.0f, -1.0f,  1.0f,         0.0f,
                                     
        -1.0f,  1.0f,  0.0f,         1.0f,
         1.0f, -1.0f,  1.0f,         0.0f,
         1.0f,  1.0f,  1.0f,         1.0f,
    };

    constexpr std::array cube{
    // X     Y      Z       Texture1     Texture2       Normal1  Normal2   Normal3
    // Back face
     -0.5f, -0.5f, -0.5f,   0.0f,        0.0f,          0.0f,    0.0f,     -1.0f,     // Bottom-left
      0.5f,  0.5f, -0.5f,   1.0f,        1.0f,          0.0f,    0.0f,     -1.0f,     // top-right
      0.5f, -0.5f, -0.5f,   1.0f,        0.0f,          0.0f,    0.0f,     -1.0f,     // bottom-right
      0.5f,  0.5f, -0.5f,   1.0f,        1.0f,          0.0f,    0.0f,     -1.0f,     // top-right
     -0.5f, -0.5f, -0.5f,   0.0f,        0.0f,          0.0f,    0.0f,     -1.0f,     // bottom-left
     -0.5f,  0.5f, -0.5f,   0.0f,        1.0f,          0.0f,    0.0f,     -1.0f,     // top-left
     // Front face                                                                    
     -0.5f, -0.5f,  0.5f,   0.0f,        0.0f,          0.0f,    0.0f,      1.0f,     // bottom-left
      0.5f, -0.5f,  0.5f,   1.0f,        0.0f,          0.0f,    0.0f,      1.0f,     // bottom-right
      0.5f,  0.5f,  0.5f,   1.0f,        1.0f,          0.0f,    0.0f,      1.0f,     // top-right
      0.5f,  0.5f,  0.5f,   1.0f,        1.0f,          0.0f,    0.0f,      1.0f,     // top-right
     -0.5f,  0.5f,  0.5f,   0.0f,        1.0f,          0.0f,    0.0f,      1.0f,     // top-left
     -0.5f, -0.5f,  0.5f,   0.0f,        0.0f,          0.0f,    0.0f,      1.0f,     // bottom-left
     // Left face                                                                     
     -0.5f,  0.5f,  0.5f,   1.0f,        0.0f,         -1.0f,    0.0f,      0.0f,     // top-right
     -0.5f,  0.5f, -0.5f,   1.0f,        1.0f,         -1.0f,    0.0f,      0.0f,     // top-left
     -0.5f, -0.5f, -0.5f,   0.0f,        1.0f,         -1.0f,    0.0f,      0.0f,     // bottom-left
     -0.5f, -0.5f, -0.5f,   0.0f,        1.0f,         -1.0f,    0.0f,      0.0f,     // bottom-left
     -0.5f, -0.5f,  0.5f,   0.0f,        0.0f,         -1.0f,    0.0f,      0.0f,     // bottom-right
     -0.5f,  0.5f,  0.5f,   1.0f,        0.0f,         -1.0f,    0.0f,      0.0f,     // top-right
     // Right face                                                                    
      0.5f,  0.5f,  0.5f,   1.0f,        0.0f,          1.0f,    0.0f,      0.0f,     // top-left
      0.5f, -0.5f, -0.5f,   0.0f,        1.0f,          1.0f,    0.0f,      0.0f,     // bottom-right
      0.5f,  0.5f, -0.5f,   1.0f,        1.0f,          1.0f,    0.0f,      0.0f,     // top-right   
      0.5f, -0.5f, -0.5f,   0.0f,        1.0f,          1.0f,    0.0f,      0.0f,     // bottom-right
      0.5f,  0.5f,  0.5f,   1.0f,        0.0f,          1.0f,    0.0f,      0.0f,     // top-left
      0.5f, -0.5f,  0.5f,   0.0f,        0.0f,          1.0f,    0.0f,      0.0f,     // bottom-left 
     // Bottom face                                                                  
     -0.5f, -0.5f, -0.5f,   0.0f,        1.0f,          0.0f,   -1.0f,      0.0f,     // top-right
      0.5f, -0.5f, -0.5f,   1.0f,        1.0f,          0.0f,   -1.0f,      0.0f,     // top-left
      0.5f, -0.5f,  0.5f,   1.0f,        0.0f,          0.0f,   -1.0f,      0.0f,     // bottom-left
      0.5f, -0.5f,  0.5f,   1.0f,        0.0f,          0.0f,   -1.0f,      0.0f,     // bottom-left
     -0.5f, -0.5f,  0.5f,   0.0f,        0.0f,          0.0f,   -1.0f,      0.0f,     // bottom-right
     -0.5f, -0.5f, -0.5f,   0.0f,        1.0f,          0.0f,   -1.0f,      0.0f,     // top-right
     // Top face                                                                     
     -0.5f,  0.5f, -0.5f,   0.0f,        1.0f,          0.0f,    1.0f,      0.0f,     // top-left
      0.5f,  0.5f,  0.5f,   1.0f,        0.0f,          0.0f,    1.0f,      0.0f,     // bottom-right
      0.5f,  0.5f, -0.5f,   1.0f,        1.0f,          0.0f,    1.0f,      0.0f,     // top-right   
      0.5f,  0.5f,  0.5f,   1.0f,        0.0f,          0.0f,    1.0f,      0.0f,     // bottom-right
     -0.5f,  0.5f, -0.5f,   0.0f,        1.0f,          0.0f,    1.0f,      0.0f,     // top-left
     -0.5f,  0.5f,  0.5f,   0.0f,        0.0f,          0.0f,    1.0f,      0.0f,     // bottom-left 

    };

    constexpr std::array cubeCoords{
        //      X      Y      Z     
                -0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                 0.5f,  0.5f, -0.5f,
                 0.5f,  0.5f, -0.5f,
                -0.5f,  0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,

                -0.5f, -0.5f,  0.5f,
                 0.5f, -0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,
                -0.5f, -0.5f,  0.5f,

                -0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,

                 0.5f,  0.5f,  0.5f,
                 0.5f,  0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,

                -0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f,  0.5f,
                 0.5f, -0.5f,  0.5f,
                -0.5f, -0.5f,  0.5f,
                -0.5f, -0.5f, -0.5f,

                -0.5f,  0.5f, -0.5f,
                 0.5f,  0.5f, -0.5f,
                 0.5f,  0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f, -0.5f,
    };

    constexpr std::array cubeTexCoords{
        //  Texture1     Texture2
            0.0f,        0.0f,   
            1.0f,        0.0f,   
            1.0f,        1.0f,   
            1.0f,        1.0f,   
            0.0f,        1.0f,   
            0.0f,        0.0f,   

            0.0f,        0.0f,   
            1.0f,        0.0f,   
            1.0f,        1.0f,   
            1.0f,        1.0f,   
            0.0f,        1.0f,   
            0.0f,        0.0f,   

            1.0f,        0.0f,   
            1.0f,        1.0f,   
            0.0f,        1.0f,   
            0.0f,        1.0f,   
            0.0f,        0.0f,   
            1.0f,        0.0f,   

            1.0f,        0.0f,   
            1.0f,        1.0f,   
            0.0f,        1.0f,   
            0.0f,        1.0f,   
            0.0f,        0.0f,   
            1.0f,        0.0f,   

            0.0f,        1.0f,   
            1.0f,        1.0f,   
            1.0f,        0.0f,   
            1.0f,        0.0f,   
            0.0f,        0.0f,   
            0.0f,        1.0f,   

            0.0f,        1.0f,   
            1.0f,        1.0f,   
            1.0f,        0.0f,   
            1.0f,        0.0f,   
            0.0f,        0.0f,   
            0.0f,        1.0f,   
    };

    constexpr std::array cubeNormals{
        //   Normal1  Normal2   Normal3
             0.0f,    0.0f,     -1.0f,
             0.0f,    0.0f,     -1.0f,
             0.0f,    0.0f,     -1.0f,
             0.0f,    0.0f,     -1.0f,
             0.0f,    0.0f,     -1.0f,
             0.0f,    0.0f,     -1.0f,

             0.0f,    0.0f,      1.0f,
             0.0f,    0.0f,      1.0f,
             0.0f,    0.0f,      1.0f,
             0.0f,    0.0f,      1.0f,
             0.0f,    0.0f,      1.0f,
             0.0f,    0.0f,      1.0f,

            -1.0f,    0.0f,      0.0f,
            -1.0f,    0.0f,      0.0f,
            -1.0f,    0.0f,      0.0f,
            -1.0f,    0.0f,      0.0f,
            -1.0f,    0.0f,      0.0f,
            -1.0f,    0.0f,      0.0f,

             1.0f,    0.0f,      0.0f,
             1.0f,    0.0f,      0.0f,
             1.0f,    0.0f,      0.0f,
             1.0f,    0.0f,      0.0f,
             1.0f,    0.0f,      0.0f,
             1.0f,    0.0f,      0.0f,

             0.0f,   -1.0f,      0.0f,
             0.0f,   -1.0f,      0.0f,
             0.0f,   -1.0f,      0.0f,
             0.0f,   -1.0f,      0.0f,
             0.0f,   -1.0f,      0.0f,
             0.0f,   -1.0f,      0.0f,

             0.0f,    1.0f,      0.0f,
             0.0f,    1.0f,      0.0f,
             0.0f,    1.0f,      0.0f,
             0.0f,    1.0f,      0.0f,
             0.0f,    1.0f,      0.0f,
             0.0f,    1.0f,      0.0f,
    };

    constexpr std::array cubeIndices{
     0u, 1u, 2u,        // Z+ back
     3u, 4u, 5u,
    
     6u, 7u, 8u,        // Z- front
     9u, 10u, 11u,

     12u, 13u, 14u,     // X+ left
     15u, 16u, 17u,

     18u, 19u, 20u,     // X- right
     21u, 22u, 23u,

     24u, 25u, 26u,     // Y- bottom
     27u, 28u, 29u,

     30u, 31u, 32u,     // Y+ top
     33u, 34u, 35u,
    };

    constexpr glm::vec3 cubePositions2[] = {
        glm::vec3(1.0f,  0.5f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(0.0f,  0.0f, 0.0f)
    };

    constexpr glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  -2.5f,  0.0f),

        glm::vec3(0.0f,  0.5f,  0.0f),
        glm::vec3(2.0f,  0.5f,  0.0f), //
        glm::vec3(4.0f,  0.5f,  0.0f),

        glm::vec3(0.0f,  2.0f,  0.0f),
        glm::vec3(0.0f,  4.0f,  0.0f), //
        glm::vec3(0.0f,  6.0f,  0.0f),

        glm::vec3(0.0f,  0.5f,  0.0f),
        glm::vec3(0.0f,  0.5f,  2.0f), //
        glm::vec3(0.0f,  0.5f,  4.0f),
    };

    constexpr std::array xyz{
         // Positions             // Colors        
         -10.0f,   0.0f,  0.0f,   1.0f, 0.0f, 0.0f,
          10.0f,   0.0f,  0.0f,   1.0f, 0.0f, 0.0f,
          0.0f,  -10.0f,  0.0f,   0.0f, 1.0f, 0.0f,
          0.0f,   10.0f,  0.0f,   0.0f, 1.0f, 0.0f,
          0.0f,    0.0f, -10.0f,  0.0f, 0.0f, 1.0f,
          0.0f,    0.0f,  10.0f,  0.0f, 0.0f, 1.0f,
    };

    constexpr std::array floor{ // Hex
        // Positions           // Texture    // Color
          0.0f,   0.0f,  0.0f, 0.5f, 0.5f,   0.2f, 0.2f, 0.2f,
          0.0f,   1.0f,  0.0f, 0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
          0.67f,  0.50f, 0.0f, 1.0f, 0.75f,  0.0f, 1.0f, 0.0f,
          0.67f, -0.50f, 0.0f, 1.0f, 0.25f,  0.0f, 0.0f, 1.0f,  
          0.0f,  -1.0f,  0.0f, 0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
         -0.67f, -0.50f, 0.0f, 0.0f, 0.25f,  1.0f, 1.0f, 0.0f,
         -0.67f,  0.50f, 0.0f, 0.0f, 0.75f,  1.0f, 0.0f, 1.0f,
    };

    constexpr std::array floor2{ // Hex
        // Positions            // Texture    // Normal
          0.0f,   0.0f,  0.0f,  0.5f, 0.5f,   0.0f, 0.0f, -1.0f,
          0.0f,   1.0f,  0.0f,  0.5f, 1.0f,   0.0f, 0.0f, -1.0f,
          0.67f,  0.50f, 0.0f,  1.0f, 0.75f,  0.0f, 0.0f, -1.0f,
          0.67f, -0.50f, 0.0f,  1.0f, 0.25f,  0.0f, 0.0f, -1.0f,
          0.0f,  -1.0f,  0.0f,  0.5f, 0.0f,   0.0f, 0.0f, -1.0f,
         -0.67f, -0.50f, 0.0f,  0.0f, 0.25f,  0.0f, 0.0f, -1.0f,
         -0.67f,  0.50f, 0.0f,  0.0f, 0.75f,  0.0f, 0.0f, -1.0f,
    };

    constexpr std::array floorIndices{
         0u, 1u, 2u,
         0u, 2u, 3u,
         0u, 3u, 4u,
         0u, 4u, 5u,
         0u, 5u, 6u,
         0u, 6u, 1u,
    };

    //constexpr std::array singleCube{
    //    //      X      Y      Z      Normal1  Normal2   Normal3
    //            -0.5f, -0.5f, -0.5f,  0.0f,    0.0f,     -1.0f,
    //             0.5f, -0.5f, -0.5f,  0.0f,    0.0f,     -1.0f,
    //             0.5f,  0.5f, -0.5f,  0.0f,    0.0f,     -1.0f,
    //             0.5f,  0.5f, -0.5f,  0.0f,    0.0f,     -1.0f,
    //            -0.5f,  0.5f, -0.5f,  0.0f,    0.0f,     -1.0f,
    //            -0.5f, -0.5f, -0.5f,  0.0f,    0.0f,     -1.0f,
    //
    //            -0.5f, -0.5f,  0.5f,  0.0f,    0.0f,      1.0f,
    //             0.5f, -0.5f,  0.5f,  0.0f,    0.0f,      1.0f,
    //             0.5f,  0.5f,  0.5f,  0.0f,    0.0f,      1.0f,
    //             0.5f,  0.5f,  0.5f,  0.0f,    0.0f,      1.0f,
    //            -0.5f,  0.5f,  0.5f,  0.0f,    0.0f,      1.0f,
    //            -0.5f, -0.5f,  0.5f,  0.0f,    0.0f,      1.0f,
    //
    //            -0.5f,  0.5f,  0.5f, -1.0f,    0.0f,      0.0f,
    //            -0.5f,  0.5f, -0.5f, -1.0f,    0.0f,      0.0f,
    //            -0.5f, -0.5f, -0.5f, -1.0f,    0.0f,      0.0f,
    //            -0.5f, -0.5f, -0.5f, -1.0f,    0.0f,      0.0f,
    //            -0.5f, -0.5f,  0.5f, -1.0f,    0.0f,      0.0f,
    //            -0.5f,  0.5f,  0.5f, -1.0f,    0.0f,      0.0f,
    //
    //             0.5f,  0.5f,  0.5f,  1.0f,    0.0f,      0.0f,
    //             0.5f,  0.5f, -0.5f,  1.0f,    0.0f,      0.0f,
    //             0.5f, -0.5f, -0.5f,  1.0f,    0.0f,      0.0f,
    //             0.5f, -0.5f, -0.5f,  1.0f,    0.0f,      0.0f,
    //             0.5f, -0.5f,  0.5f,  1.0f,    0.0f,      0.0f,
    //             0.5f,  0.5f,  0.5f,  1.0f,    0.0f,      0.0f,
    //
    //            -0.5f, -0.5f, -0.5f,  0.0f,   -1.0f,      0.0f,
    //             0.5f, -0.5f, -0.5f,  0.0f,   -1.0f,      0.0f,
    //             0.5f, -0.5f,  0.5f,  0.0f,   -1.0f,      0.0f,
    //             0.5f, -0.5f,  0.5f,  0.0f,   -1.0f,      0.0f,
    //            -0.5f, -0.5f,  0.5f,  0.0f,   -1.0f,      0.0f,
    //            -0.5f, -0.5f, -0.5f,  0.0f,   -1.0f,      0.0f,
    //
    //            -0.5f,  0.5f, -0.5f,  0.0f,    1.0f,      0.0f,
    //             0.5f,  0.5f, -0.5f,  0.0f,    1.0f,      0.0f,
    //             0.5f,  0.5f,  0.5f,  0.0f,    1.0f,      0.0f,
    //             0.5f,  0.5f,  0.5f,  0.0f,    1.0f,      0.0f,
    //            -0.5f,  0.5f,  0.5f,  0.0f,    1.0f,      0.0f,
    //            -0.5f,  0.5f, -0.5f,  0.0f,    1.0f,      0.0f,
    //};

}