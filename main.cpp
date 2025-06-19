#include <cstdio>
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>

GLuint VAO, VBO;
GLuint shader_id;

// --- Shaders ---
// Vertex Shader Source Code (GLSL)
const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos; // 'aPos' is the vertex position, linked to location 0

    void main()
    {
        gl_Position = vec4(aPos, 1.0); // Output the position in clip space
        gl_PointSize = 10.0f;          // Set the point size in pixels (requires glEnable(GL_PROGRAM_POINT_SIZE))
    }
)";

// Fragment Shader Source Code (GLSL)
const char *fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor; // The output color of this fragment

    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // Orange color (R, G, B, Alpha)
    }
)";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void processInput(GLFWwindow *window);
void compileShaders();
void setupPointBuffers(float* vertices);

void renderPoints() {
    glUseProgram(shader_id);
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);
}

int main() {

    const size_t width = 800;
    const size_t height = 600;

    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
    GLFWwindow* window = glfwCreateWindow(width, height, "loading-screen", NULL, NULL);
    if(!window){
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD");
        return -1;
    }

    glViewport(0, 0, width, height);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distrib(-1.0f, 1.0f);

    float x = distrib(gen);
    float y = distrib(gen);

    // Vertex data for a single point (at the origin, for example)
    float vertices[] = {
        x, y, 0.0f // X, Y, Z coordinates
    };

    compileShaders();
    setupPointBuffers(vertices);

    glEnable(GL_PROGRAM_POINT_SIZE);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderPoints();

        glfwPollEvents();    
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_id);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void compileShaders() {
    // Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for vertex shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    // Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for fragment shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    // Shader Program
    shader_id = glCreateProgram();
    glAttachShader(shader_id, vertexShader);
    glAttachShader(shader_id, fragmentShader);
    glLinkProgram(shader_id);
    // Check for shader program linking errors
    glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_id, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    // Delete the shader objects once they're linked into the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void setupPointBuffers(float* vertices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // Enable the vertex attribute array

    // Unbind the VBO and VAO (optional, but good practice to avoid accidental modification)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}