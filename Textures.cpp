//libs
#ifdef _WIN32
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "SOIL.lib")
#endif

//includes
#ifdef _WIN32
#define GLEW_STATIC
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <thread>
#include <fstream>
#include <sstream>
#include <iostream>
#endif

//Messy unoptimized function to compile a shader and outputing errors
GLuint compileShader(const std::string file_name, int shader_type, bool print_error = true)
{
    GLuint shader = glCreateShader(shader_type); //Create shader
    std::ifstream shader_file(file_name); //Read file
    std::stringstream file_stream;
    if (shader_file)
    {
        file_stream << shader_file.rdbuf();
    }
    else
    {
        std::cout << "Reading '" + file_name + "' faled!\n";
        return 0;
    }
    shader_file.close();
    std::string content = file_stream.str();
    const char* source = content.c_str();
    glShaderSource(shader, 1, &source, NULL); //Copy shader to card
    glCompileShader(shader); //Compile
    int status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == false)
    {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        std::cout << "Compiling '" + file_name + "' failed:\n" << buffer << "\n";
        return 0;
    }
    return shader;
}

int main(int argc, char* args[])
{
    //glfw stuff
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Textures", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    //glew stuff
    glewExperimental = true;
    glewInit();

    //Vertices
    float vertices[] =
    {
        -.5f, .5f, 1.f, 1.f, 1.f, 0.f, 0.f,
        .5f, .5f, 1.f, 1.f, 1.f, 1.f, 0.f,
        .5f, -.5f, 1.f, 1.f, 1.f, 1.f, 1.f,
        -.5f, -.5f, 1.f, 1.f, 1.f, 0.f, 1.f
    };

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Elements
    float elements[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    GLuint element_buffer;
    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    //Texture
    GLuint basic_texture;
    glGenTextures(1, &basic_texture);
    glBindTexture(GL_TEXTURE_2D, basic_texture);

    //Wraping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //Actually load the stuff
    int width = 0, height = 0;
    unsigned char* soil_image = SOIL_load_image("Data/img_test.png", &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, soil_image);
    SOIL_free_image_data(soil_image);

    //Shaders
    GLuint vertex_shader = compileShader("Shaders/textures.vec", GL_VERTEX_SHADER);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glLinkProgram(shader_program);
    glUseProgram(shader_program);

    //Vertex array buffer
    GLuint vao;
    glGenBuffers(1, &vao);
    glBindVertexArray(vao);

    //Attributes

    //Main loop
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}