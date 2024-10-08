//const int TICKS_PER_SECOND = 25;
//const int SKIP_TICKS_FOR_DURATION = 1000 / TICKS_PER_SECOND; // “SKIP_TICKS_FOR_DURATION”, or rather just “FRAME_DURATION” and/or “COMPUTE_DURATION”. 
//const int MAX_FRAMESKIP = 5;
//
//DWORD next_game_tick = GetCurrentTimeInMs();
//int loops;
//float interpolation;
//
//while (true) {
//
//    loops = 0;
//    while (GetCurrentTimeInMs() > next_game_tick && loops < MAX_FRAMESKIP) {
//        update_game();
//
//        next_game_tick += SKIP_TICKS_FOR_DURATION;
//        loops++;
//    }
//
//    interpolation = float(GetCurrentTimeInMs() + SKIP_TICKS_FOR_DURATION - next_game_tick) / float(SKIP_TICKS_FOR_DURATION);
//    display_game(interpolation);
//}



// .size() returns the number of elements in the vector.
//
// sizeof gives you the number of bytes used by the object definition (based on the declaration of the vector class)
//
// .size() * sizeof(type) = total size in bytes
// 
// //
// 
//constexpr std::array cubeIndices{
//     bl, tr, br,        // Z+ back
//     tr, bl, tl,
//
//     bl, br, tr,        // Z- front
//     tr, tl, bl,
//
//     tr, tl, blu,     // X+ left
//     blu, bru, tru,
//
//     tlu, bru, tru,     // X- right
//     bru, tlu, blu,
//
//     tru, tlu, blu,     // Y- bottom
//     blu, bru, tru,
//
//     tlu, bru, tru,     // Y+ top
//     bru, tlu, blu,
//};
//
//constexpr std::array cubeIndices{
//     0u, 1u, 2u,        // Z+ back
//     1u, 0u, 3u,
//
//     4u, 5u, 6u,        // Z- front
//     6u, 7u, 4u,
//
//     8u, 9u, 10u,     // X+ left
//     10u, 11u, 8u,
//
//     12u, 13u, 14u,     // X- right
//     13u, 12u, 15u,
//
//     16u, 17u, 18u,     // Y- bottom
//     18u, 19u, 16u,
//
//     20u, 21u, 22u,     // Y+ top
//     21u, 20u, 23u,
//};

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


// FLASHLIGHT EMISSION 3 WAYS
//
//vec3 CalcFlashLight(FlashLight light)
//{
//    vec3 lightDir = normalize(light.origin - vs_out.FragPosView);
//    // diffuse
//    float diff = max(dot(normalView, lightDir), 0.0f);
//    vec3 diffuse = light.diffuse * diff * textureDiffuse;
//    // specular
//    vec3 reflectDir = reflect(-lightDir, normalView);
//    float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
//    vec3 specular = light.diffuse * spec * textureSpecular;
//    // emission calculations, only when flashlight is on
//    vec3 textureEmissionMap = vec3(texture(material.flashlightMap, vs_out.TexCoords));
//    vec3 textureEmissionResult = vec3(texture(material.flashlightResult, vs_out.TexCoords));
//    // #1. emission: using specularMap as stamp, with if statement
//    //vec3 emission = vec3(0.0f);                         // Default no textureEmission visible
//    //if (textureSpecular.r == 0.0f) {                    // if textureSpecular == black (or whatever you choose)
//    //    emission = light.emission * textureEmission;    // show textureEmission
//    //}
//    // #2. emission: using specularMap as stamp, no if statement, a bit quicker, but less flexible (disable textureEmissionResult calculation)
//    //vec3 emission = textureSpecular.r * light.emission * textureEmissionResult;
//    // #3. emission: using specific emissionMap as stamp, a bit slower
//    vec3 emission = textureEmissionMap.r * light.emission * textureEmissionResult;
//    // cone
//    vec3 cameraDirection = vec3(0.0f, 0.0f, 1.0f); // camera.m_front with negated z-axis
//    float theta = dot(lightDir, cameraDirection);
//    float intensity = clamp((theta - light.outerCutOff) / light.epsilon, 0.0f, 1.0f);
//    diffuse *= intensity;
//    specular *= intensity;
//    emission *= intensity;
//    // attenuation
//    float distance = length(light.origin - vs_out.FragPosView);
//    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//    diffuse *= attenuation;
//    specular *= attenuation;
//    emission *= attenuation;
//
//    return (diffuse + specular + emission) * light.strength;
//}