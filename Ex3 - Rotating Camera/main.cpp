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

        // Run 3 times To draw 3 squares
        // this part isn't responsible for the rotation effect (the one responsible is the view matrix)
        for(int z = -1; z <= 1; z++){
            // In this tutorial, the model matrix only do translation to the square
            // Translates the square in the z-axis only
            
            // First run, z=-1, translates a square to 1 unit out the z direction
            // Second run, z=0, No translation, the square is at original location
            // First run, z=1, translates a square to 1 unit in the z direction
            
            // Froming the matrix that will change from local space to homogenous clip space
            glm::mat4 MVP = projection * view * glm::translate(
                glm::mat4(1.0f),
                glm::vec3(0, 0, z)
            );

            // First Param: Location of the uniform mvp matrix
            // Second param: 1 matrix will be sent
            // Third Param: transpose?
            // Fourth PAram: float pointer to the data to be sent
            glUniformMatrix4fv(mvpLoc, 1, false, (float*)&MVP);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
        }


        // The Translation matrix
        // ----------------
        // glm::translate
        // Takes matrix and vector, mulitply them to Build a translation matrix.
        // this translation matrix will then be sent to shader to be multiplied by the vertices and translate them.

        // First Param:  m:	Input matrix
        // Second param: v: Coordinates of a translation vector.
        // This function builds the translation matrix by performing multiplication: m*v
        
        // First Param: m: glm::mat4(1.0f)
        // If there is a single scalar parameter to a matrix constructor,
        // it is used to initialize all the components on the matrix's diagonal,
        // with the remaining components initialized to 0.0.
        // Thus the glm::mat4(1.0f) creates a 4*4 identity matrix

        // Second param: v: vec3(0,0,z)
        // The translation vector v will be put inside a translation matrix (t) as follows
        //      c0  c1  c2  c3 
        // ---------------------  
        // r0:   1   0   0  v[0]  
        // r1:   0   1   0  v[1]
        // r2:   0   0   0  v[2]
        // r3:   0   0   0  1  

        // Then the translate function will multiply m*t

        // To sum-up:
        // First param is an input matrix: m
        // Second param is a translation vector: v
        // This translation vector v will turn to a translation matrix t having the above form
        // Then returns input matrix*translation matrix

        // In the above loop we multiplied the identity with translation matrix having translation at z only
        // Result is a model matrix which causes only translation at the z coordinate.

        // Some notes:
        // matrix[0] is the left column of matrix.
        // matrix[2][1] is in the 3rd row, 2nd column.
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
