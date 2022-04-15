#include <iostream>
#include <fstream>
#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

// GLM is a mathematics library.

GLuint loadShader(const std::string& filePath, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);

    std::ifstream file(filePath);
    std::string sourceCode = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char* sourceCodeCStr = sourceCode.c_str();

    glShaderSource(shader, 1, &sourceCodeCStr, nullptr);
    glCompileShader(shader);

    return shader;
}

struct Vertex {
    float x, y, z;
    uint8_t r, g, b, a;
};

int main(int, char**) {
    
    if(!glfwInit()){
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    const int W = 800, H = 600;
    GLFWwindow* window = glfwCreateWindow(W, H, "Example 1", nullptr, nullptr);
    if(!window){
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    gladLoadGL(glfwGetProcAddress);

    GLuint program = glCreateProgram();
    
    GLuint vs = loadShader("assets/shaders/simple.vert", GL_VERTEX_SHADER);
    glAttachShader(program, vs);
    glDeleteShader(vs);

    GLuint fs = loadShader("assets/shaders/simple.frag", GL_FRAGMENT_SHADER);
    glAttachShader(program, fs);
    glDeleteShader(fs);

    glLinkProgram(program);

    GLint mvpLoc = glGetUniformLocation(program, "MVP");

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Square coordinates in local space
    Vertex vertices[] = {
        {-0.5f, -0.5f, 0.0f,   0, 255, 255, 255},
        { 0.5f, -0.5f, 0.0f, 255,   0, 255, 255},
        { 0.5f,  0.5f, 0.0f, 255, 255,   0, 255},
        {-0.5f,  0.5f, 0.0f, 255,   0,   0, 255}
    };

    glBufferData(GL_ARRAY_BUFFER, 4*sizeof(Vertex), vertices, GL_STATIC_DRAW);

    GLint positionLoc = 0; 
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);

    GLint colorLoc = 1; 
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void*)offsetof(Vertex, r));

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    uint16_t elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(uint16_t), elements, GL_STATIC_DRAW);

    while(!glfwWindowShouldClose(window)){
        
        glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(program);
        
        float angle = (float)glfwGetTime();

        // Forming the View matrix
        // This matrix changes from the world space to the camera space
        // First Param: Location of the camera in the world (eye position)
        // Second param: Where is the camera looking? (gaze direction)
        // Third Param: The view up vector of the camera (it specifies the orientation of the camera)
        
        // Note the eye position (x,z) of the camera changes by time
        // But the camera y is fixed
        // It looks at the origin, (remember the square coordinates we specified is centered at origin)
        // Thus the camera will be rotating around the y-axis.
        glm::mat4 view = glm::lookAt(
            glm::vec3(2*glm::sin(angle), 1, 2*glm::cos(angle)),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
        );

        // Perspective matrix Changes from the camera space to homogenous clip space
        // First Param: Field of view angle, when it decreases it seems where zooming in
        // Second Param: aspect ratio of the window
        // Third Param: the Near plane (The nearest distance I can't see before)
        // Fourth Param: the Far plane (The farthest distance I can't see after)
        
        // If the aspect ratio is 1.0f then the square will be drawn as rectangle
        // Why? Because where specifying the position in normalized coordinates
        // When the normalized (1 to 1) will be transformed to viewport space (of a 800*600 window)
        // the square will be stretched to a rectangle, 
        // this is why we set the aspect ratio correct to maintain squares to still be squares
        
        glm::mat4 projection = glm::perspective(
            glm::pi<float>()/2,
            W/float(H),
            0.01f,
            100.0f
        );

        // Froming the matrix that will change from local space to normalized view volume
        // Note there's no model matrix here, i.e no transformation is done on the object
        
        // Also, The object will be at 0,0,0 in the world space
        // Since the vertices coordinates specified for the square is centered at 0,0,0 (check vertices array above)

        glm::mat4 VP = projection * view;
        // First Param: Location of the uniform mvp matrix
        // Second param: 1 matrix will be sent
        // Third Param: transpose?
        // Fourth PAram: float pointer to the data to be sent
        glUniformMatrix4fv(mvpLoc, 1, false, (float*)&VP);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
