#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <array>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int width = 1600;
    const int height = 1200;

    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << '\n';
        return -1;
    }

    //

    int success;
    char infoLog[512];

    // Vertex shader 0

    const char* vertexShader0Source{
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0" };

    unsigned int vertexShader0{ glCreateShader(GL_VERTEX_SHADER) };
    glShaderSource(vertexShader0, 1, &vertexShader0Source, nullptr);
    glCompileShader(vertexShader0);

    glGetShaderiv(vertexShader0, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader0, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
    }

    // Fragment shader 0

    //const char* fragmentShader0Source{
    //"#version 330 core\n"
    //"out vec4 FragColor;\n"
    //"void main()\n"
    //"{\n"
    //"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    //"}\n\0" };

    //unsigned int fragmentShader0{ glCreateShader(GL_FRAGMENT_SHADER) };
    //glShaderSource(fragmentShader0, 1, &fragmentShader0Source, nullptr);
    //glCompileShader(fragmentShader0);

    //glGetShaderiv(fragmentShader0, GL_COMPILE_STATUS, &success);
    //if (!success) {
    //    glGetShaderInfoLog(fragmentShader0, 512, nullptr, infoLog);
    //    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
    //}

    // Fragment shader 1

    const char* fragmentShader1Source{
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0" };

    unsigned int fragmentShader1{ glCreateShader(GL_FRAGMENT_SHADER) };
    glShaderSource(fragmentShader1, 1, &fragmentShader1Source, nullptr);
    glCompileShader(fragmentShader1);

    glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader1, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
    }

    // link shaders 0 + 0

    //unsigned int shaderProgram0{ glCreateProgram() };
    //glAttachShader(shaderProgram0, vertexShader0);
    //glAttachShader(shaderProgram0, fragmentShader0);
    //glLinkProgram(shaderProgram0);

    //glGetProgramiv(shaderProgram0, GL_LINK_STATUS, &success);
    //if (!success) {
    //    glGetProgramInfoLog(shaderProgram0, 512, nullptr, infoLog);
    //    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
    //}

    // link shaders 0 + 1

    unsigned int shaderProgram1{ glCreateProgram() };
    glAttachShader(shaderProgram1, vertexShader0);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);

    glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram1, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
    }

    glDeleteShader(vertexShader0);
    //glDeleteShader(fragmentShader0);
    glDeleteShader(fragmentShader1);

    // Vertex data

    //constexpr std::array vertices {
    //  0.5f,  0.5f, 0.0f, // top right
    //  0.5f, -0.5f, 0.0f, // bottom right
    // -0.5f,  -0.5f, 0.0f, // bottom left
    // -0.5f,  0.5f, 0.0f, // top left 
    //};

    //constexpr std::array indices {
    // 0u, 1u, 3u,  // first Triangle
    // 1u, 2u, 3u   // second Triangle
    //};

    constexpr std::array vertices0{
     -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     -0.25f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      0.25f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    constexpr std::array indices0{
     0u, 1u, 2u,  // first Triangle
     2u, 3u, 4u   // second Triangle
    };

    constexpr std::array vertices1{
     -0.5f,   0.0f, 0.0f,
     -0.25f, -0.5f, 0.0f,
      0.0f,   0.0f, 0.0f,
      0.25f, -0.5f, 0.0f,
      0.5f,   0.0f, 0.0f,
    };

    constexpr std::array indices1{
     0u, 1u, 2u,  // first Triangle
     2u, 3u, 4u   // second Triangle
    };

    unsigned int vao0; // vertex array object
    glGenVertexArrays(1, &vao0);
    glBindVertexArray(vao0);

    unsigned int vbo0; // vertex buffer object
    glGenBuffers(1, &vbo0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), &vertices0, GL_STATIC_DRAW);

    unsigned int ebo0; // element buffer object
    glGenBuffers(1, &ebo0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices0), &indices0, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    //

    unsigned int vao1; // vertex array object
    glGenVertexArrays(1, &vao1);
    glBindVertexArray(vao1);

    unsigned int vbo1; // vertex buffer object
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), &vertices1, GL_STATIC_DRAW);

    //unsigned int ebo1; // element buffer object
    //glGenBuffers(1, &ebo1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices0), &indices0, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    //int ourColorLocation = glGetUniformLocation(shaderProgram1, "ourColor");

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram1);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(vao0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUseProgram(shaderProgram1);
        //float redValue = (sin(static_cast<float>(glfwGetTime())) / 8.0f) + 0.5f;
        //float greenValue = (sin(static_cast<float>(glfwGetTime())) / 2.0f) + 0.5f;
        //glUniform4f(ourColorLocation, redValue, greenValue, 0.0f, 1.0f);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(vao1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao0);
    glDeleteBuffers(1, &vbo0);
    glDeleteBuffers(1, &ebo0);
    glDeleteVertexArrays(1, &vao1);
    glDeleteBuffers(1, &vbo1);
    //glDeleteBuffers(1, &ebo1);
    //glDeleteProgram(shaderProgram0);
    glDeleteProgram(shaderProgram1);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

#pragma warning( suppress : 4100 ) //#pragma warning( default : 4100 )
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}