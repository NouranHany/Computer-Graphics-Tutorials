#include <iostream>
#include <fstream>
#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

GLuint loadShader(const std::string& filePath, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);

    std::ifstream file(filePath);
    std::string source = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char* sourceCStr = source.c_str();

    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    return shader;
}

// We've created this struct so that allocate uint to the color channels 
// Color channels only need 1 byte (uint8_t), since take values from 0-255
// No need to allocate float for color channels (4 bytes)
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

    GLFWwindow* window = glfwCreateWindow(500, 500, "Example 1", nullptr, nullptr);
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

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Position of vertix merged with its color data interleaved
    // It's important to put the data of a vertix together besides it. 
    Vertex vertices[] = {
        {-0.5f, -0.5f, 0.0f,   0, 255, 255, 255},
        { 0.5f, -0.5f, 0.0f, 255,   0, 255, 255},
        { 0.5f,  0.5f, 0.0f, 255, 255,   0, 255},
        {-0.5f,  0.5f, 0.0f, 255,   0, 255, 255},
    };

    // Creates 1 vertix buffer object 
    GLuint VBO;
    glGenBuffers(1, &VBO);
    // Needs to bind the buffer first, before using it.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // This function is to put the data, defined in main.cpp, into the VBO
    // Note that we've sent the vertices data only one time.
    // After its placed on the vram, every time the gpu will draw using it.
    // We specify the buffer's size when we send it data
    
    // First Param, specifies the type of the buffer to put the data in
    // Second Param: State the size of the data in bytes:
    // 4*(3*4 + 4*1), i.e 4 vertices (each of 3 positions (x,y,z) each is float i.e 4 bytes and each vertix have 4 color channels each is 1 byte).
    // Third param: the data to put in the buffer
    // Fourth Param: means want to use this buffer for drawing and this data is not intended to change (static)
    glBufferData(GL_ARRAY_BUFFER, 4*sizeof(Vertex), vertices, GL_STATIC_DRAW);


    // The commented line below, gets the location of the in variable position found in the shader
    // the location of the variable is obtained from the attribute memory
    // GLint positionLoc = glGetAttribLocation(program, "position");
    
    GLint positionLoc = 0;
    // Need to enable the attribute
    glEnableVertexAttribArray(positionLoc);
    // Specifies how to read the data from the buffer and send it to position attribute (found in the vshader)
    // First param: the location of the variable to
    // Second param: How many componenets to be sent each run? 3 (x,y,z)
    // Third param: Type of each component? float
    // Fourth param: Normalized? if true will map the colors from 0-maxSizeOf unit8_t (0-255)  to 0-1 since shaders uses this color range.
    // Fifth param: Stride? After how many bytes Will find the next component
    // Sixth param: offset? The # of bytes to skip before reading the positions
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
    // But how this line knows to which buffer this description belongs ?
    // It belongs to the last binded buffer. 

    GLint colorLoc = 1; //glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(colorLoc);
    // Sixth param: offset? The # of bytes to skip before reading the colors
    // offsetof(Vertex, r) specifies the # of bytes before the r varibale in the struct (i.e 12 bytes)
    glVertexAttribPointer(colorLoc, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void*)offsetof(Vertex, r));

    // Holds how to form traingles from vertices
    // Which vertices it will take from the vertices array to form each face
    
    // Elements at 0,1,2 indeces in vertices form 1 triangle
    // Elements at 2,3,0 indeces in vertices form another triangle
    // This way we need to store 4 vertices instead of 6 to draw 2 traingles as square
    // We're optimizing in the memory storage of the GPU 
    uint16_t elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint EBO;
    // Create an Element Buffer Object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(uint16_t), elements, GL_STATIC_DRAW);

    glBindVertexArray(0);

    GLint timeLoc = glGetUniformLocation(program, "time");

    while(!glfwWindowShouldClose(window)){
        // We're writing numbers here ended with f
        // Since this function's signature takes floats
        glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(program);

        glUniform1f(timeLoc, (float)glfwGetTime());

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        // Want to draw 6 elements
        // Third param: Type of data in the elements array
        // Fourth param: To skip some locations in the buffer 
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
