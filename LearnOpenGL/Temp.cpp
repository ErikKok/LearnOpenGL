
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


//// Floor Main

//VertexArray floorVao;
//VertexBuffer floorVbo(&Data::floor2, sizeof(Data::floor2));
//VertexAttributeLayout floorlayout{};
//floorlayout.pushVertexAttributeLayout<float>(3);
//floorlayout.pushVertexAttributeLayout<float>(2);
//floorlayout.pushVertexAttributeLayout<float>(3);
//floorVao.addVertexAttributeLayout(floorVbo, floorlayout);
//ElementBuffer floorEbo(&Data::floorIndices, sizeof(Data::floorIndices));

//Texture texture2("Textures\\floor.jpg");

////Shader floorShader("Shaders\\floor.shader");
////floorShader.useShader();
////floorShader.setInt("texture2", 2);

//// Normalize test
////glm::vec3 test = glm::vec3(7.3f,0.5f, 1.8f);
////glm::vec3 test2 = glm::normalize(test);
////std::println("test: {}, {}, {}", test.x, test.y, test.z);
////std::println("test normalized: {}, {}, {}", test2.x, test2.y, test2.z);

//// Floor Renderloop
//
////spotlight.useShader();
////texture2.bindTexture(2);
//floorVao.bindVertexArray();
//model = glm::mat4(1.0f);
////model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//model = glm::rotate(model, glm::radians(95.0f), glm::vec3(1.0, 0.0, 0.0));
//model = glm::scale(model, glm::vec3(25.0, 25.0, 2.0));
//multiLight.setMat4("model", model);
///////
//multiLight.setMat3("NormalViewCPU", glm::transpose(glm::inverse(view * model)));
////multiLight.setMat3("NormalWorldCPU", glm::transpose(glm::inverse(model)));
///////
//glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Data::floor2.size()), GL_UNSIGNED_INT, 0);

////model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
////for (unsigned int i = 0; i < 32; i++) {
////    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -0.0f));
////    
////    //model = glm::scale(model, glm::vec3(20.0, 20.0, 20.0));
////    floorShader.setMat4("model", model);
////    for (unsigned int j = 0; j < 8; j++) {
////        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -0.0f));

////        //model = glm::scale(model, glm::vec3(20.0, 20.0, 20.0));
////        floorShader.setMat4("model", model);
////        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(floor.size()), GL_UNSIGNED_INT, 0);
////    }
////    model = glm::translate(model, glm::vec3(0.0f, -8.0f, -0.0f));
////    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(floor.size()), GL_UNSIGNED_INT, 0);
////}


////////////////////////////////////////////////////////////////////////////////////////////////////
