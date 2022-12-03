#include <glad/glad.h>  // Must be included before GLFW
#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

bool wireframe = false;

// callback handling window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// callback handling keyboard input
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main() {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window, this is done by glfw
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // make the window current context
    glfwMakeContextCurrent(window);

    // load glad
    // opengl is a specification implemented by several different drivers, thus it does not know
    // the location of the functions at compile time. Glad will help us to this so we dont have
    // to do it ourselves.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell opengl the size of the rendering window
    glViewport(0, 0, 1200, 900);

    // register callback handling resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // register callback for handling keyboard presses
    glfwSetKeyCallback(window, key_callback);

    // build shaders which are small steps that processes the data
    // some shaders are configurable and allows the developer to write their own

    // vertex shaders transforms 3D coordinates to different 3D coordinates, and the vertex shader allows
    // us to do some basic processing on the vertex attributes
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // attach shader source to shader object and compile it
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader is to calculate the final color of a pixel and this is usually the stage where all the advanced
    // OpenGL effects occur usually the fragment shader contains data about the 3D scene that it can use to calculate
    // the final pixel color (like lights, shadows, color of the light and so on)
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    // attach shaders and link
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // now we do not need shader objects anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {0.9f,  0.0f, 0.0f, 0.5f,  0.0f, 0.0f, 0.7f,  0.5f, 0.0f,

                        -0.9f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f, -0.7f, 0.5f, 0.0f};

    // generate vertex object buffer that can hold vertices in the GPU memory
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // vertex array object stores subsequent vertex attribute calls
    // when configuring vertex attribute pointers you only have to make those calls once and whenever we want to draw
    // the object, we can just bind the corresponding VAO this makes switching between different vertex data and
    // attribute configurations as easy as binding a different VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
    //         1. bind Vertex Array Object
    glBindVertexArray(VAO);

    //         2. copy our vertices array in a buffer for OpenGL to use
    // bind buffer to the buffer type for vertices
    // only one buffer can be bind to GL_ARRAY_BUFFER at a time
    // several buffers of different type can be bound
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // copy vertex data to opengl buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //          3. then set our vertex attributes pointers
    // tell opengl how to interpret the data
    // The position data is stored as 32-bit (4 byte) floating point values.
    // Each position is composed of 3 of those values.
    // There is no space (or other values) between each set of 3 values. The values are tightly packed in the array.
    // The first value in the data is at the beginning of the buffer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        //          4. draw the object
        glUseProgram(shaderProgram);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// on window resize, viewport should also be resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        if (!wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            wireframe = true;
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            wireframe = false;
        }
    }
}
