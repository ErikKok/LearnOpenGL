
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

//////// FRAMEBUFFERSSSS

    /////////////////////////////////////
    ////// Framebuffer //////////////////
//std::println("CREATE Framebuffer");//
//
//unsigned int framebuffer;
//glGenFramebuffers(1, &framebuffer);
//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//
//// generate texture
//unsigned int textureColorbuffer;
//glGenTextures(1, &textureColorbuffer);
//glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Global::windowWidth, Global::windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//glBindTexture(GL_TEXTURE_2D, 0);
//
//// attach it to currently bound framebuffer object
//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
//
//unsigned int rbo;
//glGenRenderbuffers(1, &rbo);
//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Global::windowWidth, Global::windowHeight);
//glBindRenderbuffer(GL_RENDERBUFFER, 0);
//
//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
//
//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//std::println("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
//glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//VertexArray framebufferVao;
//VertexBuffer framebufferVbo(sizeof(Data::framebuffer), &Data::framebuffer);
//VertexAttributeLayout framebufferlayout{};
//framebufferlayout.pushVertexAttributeLayout<float>(2);
//framebufferlayout.pushVertexAttributeLayout<float>(2);
//framebufferVao.addVertexAttributeLayout(framebufferVbo, framebufferlayout);
//
//Shader framebufferShader("Shaders\\framebuffer.shader");
//
///////////////////////
//
//unsigned int framebuffer2;
//glGenFramebuffers(1, &framebuffer2);
//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
//
//// generate mirror texture
//unsigned int textureColorbufferMirror;
//glGenTextures(1, &textureColorbufferMirror);
//glBindTexture(GL_TEXTURE_2D, textureColorbufferMirror);
//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Global::windowWidth, Global::windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//glBindTexture(GL_TEXTURE_2D, 0);
//
//// attach it to currently bound framebuffer object
//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbufferMirror, 0);
//
//unsigned int rbo2;
//glGenRenderbuffers(1, &rbo2);
//glBindRenderbuffer(GL_RENDERBUFFER, rbo2);
//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Global::windowWidth, Global::windowHeight);
//glBindRenderbuffer(GL_RENDERBUFFER, 0);
//
//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo2);
//
//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//std::println("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
//glBindFramebuffer(GL_FRAMEBUFFER, 0);


//////// VOORBEELD 2X DRAW CALL

//// eerste keer andere buffer binden, eerst e.e.a. clearen
//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
//// clear framebuffer2 once per renderloop
//glClearColor(0.5f, 0.2f, 0.2f, 1.0f);
//
//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//Global::clearStencilBuffer();

// originele 1e draw call
//glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Data::cube.size()), GL_UNSIGNED_INT, 0);
//
//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
//
//// re-initialize the view with a reverseCamera, and update uniforms
//view = Global::camera.GetReverseViewMatrix();
//modelView = view * model;
//multiLight.setMat4("modelView", modelView);
//multiLight.setMat3("NormalViewCPU", glm::transpose(glm::inverse(modelView)));
//
//// draw to textureColorbufferMirror
//glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Data::cube.size()), GL_UNSIGNED_INT, 0);
//
//// back to default/first framebuffer
//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//view = Global::camera.GetViewMatrix();


//////// EIND RENDERLOOP

        /////////////////////////////
        /////////////////Framebuffers

//        // Init default Framebuffer
//glBindFramebuffer(GL_FRAMEBUFFER, 0); // render to default framebuffer
//// clear all relevant buffers
//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
//glClear(GL_COLOR_BUFFER_BIT);
//
//glDisable(GL_DEPTH_TEST); // disable depth test so a-space quad isn't discarded due to depth test.
//
//// Framebuffer / textureColorbuffer
//framebufferShader.useShader();
//framebufferShader.setMat4("model", glm::mat4(1.0f)); // no tranfsormation, just fullscreen quad
//framebufferVao.bindVertexArray();
//
//glActiveTexture(GL_TEXTURE0); // activate default texture unit (only needed when using multi-textures)
//glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
//glDrawArrays(GL_TRIANGLES, 0, 6);  // render (fullscreen) quad
//
//// Framebuffer 2 / texture textureColorbufferMirror
//// lightning is all wrong in this framebuffer, that should be negated or something...
//// also it's not really a rearviewmirror, there is no horizontal flipping, just looking backwards
//model = glm::mat4(1.0f);
//model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.0f)); // you could probably also reduce the texture resolution with this amount
//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.0f));
//framebufferShader.setMat4("model", model); // transform quad
//
//glBindTexture(GL_TEXTURE_2D, textureColorbufferMirror);
//glDrawArrays(GL_TRIANGLES, 0, 6);
//
//glEnable(GL_DEPTH_TEST);

////////////////////////////////////////////////////////////////////////////////////////////////////


// MOVE CONSTRUCTOR TEXTURE, ZET TYPE OP 'MOVED'

//Texture(Texture&& other) noexcept = delete; 					// Move constructor	
//	:m_id{ other.m_id }
//	,m_type{ other.m_type }
//	,m_filePath{ other.m_filePath }
//	,m_fileName{ other.m_fileName }
//	,m_width{ other.m_width }
//	,m_height{ other.m_height }
//{
//	other.m_id = 0u;
//	other.m_type = "moved"; // TODO
//	other.m_filePath = "";
//	other.m_fileName = "";
//	other.m_width = 0u;
//	other.m_height = 0u;

//	std::println("********************MOVE CONSTRUCTOR texture id: {}", m_id);
//}

//Texture& operator=(Texture&& other) noexcept = delete;		// Move assignment
//{		
//	if (&other == this)
//		return *this;

//	m_id = other.m_id;
//	m_type = other.m_type;
//	m_filePath = other.m_filePath;
//	m_fileName = other.m_fileName;
//	m_width = other.m_width;
//	m_height = other.m_height;

//	other.m_id = 0u;
//	other.m_type = "moved"; // TODO
//	other.m_filePath = "";
//	other.m_fileName = "";
//	other.m_width = 0u;
//	other.m_height = 0u;

//	std::println("********************MOVE ASSIGNMENT texture id: {}", m_id);

//	return *this;
//}

    // EN DAN MET DEZE DESTRUCTOR

//Texture::~Texture() // TODO - hacky... use smart pointers I guess? Wordt niet meer gebruikt nu!?
//{
//    std::println("************************************** ", m_id);
//    if (m_type == "moved") {
//        std::println("DELETE texture - Texture object IS deleted, but texture itself NOT! Original texture is still in use by another object!");
//    }
//    else {
//        std::println("************************************** DELETE texture id: {}", m_id);
//        glDeleteTextures(1, &m_id);
//        Global::glCheckError();
//    }
//}

// WERKTE GOED VOLGENS MIJ