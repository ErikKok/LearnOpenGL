
////// SingleCube Main

////VertexArray singleCubeVao;
//VertexBuffer cubeVbo(&Data::cube, sizeof(Data::cube));
////VertexAttributeLayout singleCubeLayout;
////singleCubeLayout.pushVertexAttributeLayout<float>(3);
////singleCubeLayout.pushVertexAttributeLayout<float>(3);
////singleCubeLayout.setVertexAttributeOffset(1, 8);
//////singleCubeLayout.setVertexStride(32);
////singleCubeVao.addVertexAttributeLayout(cubeVbo, singleCubeLayout);

////Shader singleCubeShader("Shaders\\singleCube.shader");
////singleCubeShader.useShader();
//glm::vec3 objectColorWhite{ 1.0f, 1.0f, 1.0f };
////singleCubeShader.setVec3("objectColor", objectColorWhite);
//////singleCubeShader.setVec3("material.ambient", objectColorWhite); // what color the surface reflects under ambient lighting
//////singleCubeShader.setVec3("material.diffuse1", objectColorWhite); // the color of the surface under diffuse lighting
////singleCubeShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // color of the specular highlight on the surface
////singleCubeShader.setFloat("material.shininess", 256.0f); // impacts the scattering/radius of the specular highlight
////// intensity vectors for each of the lighting components
////singleCubeShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
////singleCubeShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
////singleCubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

//// Single Cube RenderLoop

////singleCubeShader.useShader();
////singleCubeShader.setVec3("lightPos", lightPos);
////model = glm::mat4(1.0f);
////model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
////singleCubeShader.setMat4("model", model);
////singleCubeVao.bindVertexArray();
////singleCubeShader.setVec3("light.ambient", ambientColor);
////singleCubeShader.setVec3("light.diffuse", diffuseColor);
////glDrawArrays(GL_TRIANGLES, 0, 36);


////////////////////////////////////////////////////////////////////////////////////////////////////

    // Normalize test
    //glm::vec3 test = glm::vec3(7.3f,0.5f, 1.8f);
    //glm::vec3 test2 = glm::normalize(test);
    //std::println("test: {}, {}, {}", test.x, test.y, test.z);
    //std::println("test normalized: {}, {}, {}", test2.x, test2.y, test2.z);

////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////
