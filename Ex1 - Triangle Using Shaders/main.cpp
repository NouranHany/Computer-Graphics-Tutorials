#include <iostream>
#include <fstream>
#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

// A function for the 2 shaders instead of writing the code inside twice
// All objects in opengl are unsigned int, this int represents an ID
/*
    filePath: path of the shaderfile location
    returns the shader
*/
GLuint loadShader(const std::string& filePath, GLenum shaderType) {
    // Creates an empty shader
    GLuint shader = glCreateShader(shaderType);

    // need to put its code in this shader obj we created
    std::ifstream file(filePath);
    // Creates an iterator of the file, then turn file content into string
    // source now have the content of the code source of the shader.
    std::string source = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    
    // This turns the source code into a char pointer
    const char* sourceCStr = source.c_str();

    // 2nd param is how many source code strings
    // 3rd param, array of strings
    // 3rd param, since we have only 1, then send a pointer at it.
    // 4th param, size of the sting >> note that streing has a nullptr at the end so he'll now its len
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    return shader;
}

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

    // Choosing the window to focus on
    glfwMakeContextCurrent(window);

    gladLoadGL(glfwGetProcAddress);

    // This program is to combine both shaders
    // Think of the program as the pipeline
    // Note that the program don't have some info related to the rasterizer, its not the pipeline exactly, but part of it.
    GLuint program = glCreateProgram();
    GLuint vs = loadShader("assets/shaders/simple.vert", GL_VERTEX_SHADER);
    glAttachShader(program, vs);
    glDeleteShader(vs);
    GLuint fs = loadShader("assets/shaders/simple.frag", GL_FRAGMENT_SHADER);
    glAttachShader(program, fs);
    glDeleteShader(fs);
    glLinkProgram(program);

    // To draw in opengl, need to define a vertex array
    // In Lab4 will use the VAO to send data into the vertix shader
    // In this lab, we'll not use the VAO, but need to define it, otherwise opengl won't allow us to draw.
    GLuint VAO; //Vertix array object
    // Firstparam: 1 means creating one vertix array
    glGenVertexArrays(1, &VAO);

    // Give it the name of the variable 'time'
    // Returns the location (ptr) of that variable
    GLint timeLoc = glGetUniformLocation(program, "time");

    // While the close button is not pressed
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.2, 0.4, 0.6, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Need to bind the VAO before drawing
        glBindVertexArray(VAO);
        // Specify which program to use when draw
        glUseProgram(program);

        // Send to the variable time the value glfwGetTime (i.e current time)
        // Note: the link stage of the program enabled us to send to the time variable defined in any object attached to the program
        // i.e this value will be sent to the 'time' variables in both the frag and the vertix shader.
        // 1f means we're going to send 1 float
        glUniform1f(timeLoc, (float)glfwGetTime());

        // First param: either traingle/line/point
        // Second param, is to specify how many indeces to skip from the start of the array
        // All indeces before will be skipped
        // Third param: if 3 draw a traingle, if 9 draw 3 traingles, if 5 draw 1 traingle and skip the 2 vertices left.
        // This takes each 3 consequent vertices and draw a traingle with them.

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Every thing drawn on the back buffer will be swapped (visible) to the curr window
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
