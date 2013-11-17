//libs
#ifdef _WIN32
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#endif

//includes
#ifdef _WIN32
#define GLEW_STATIC
#include <gl/glew.h>
#include <GLFW/glfw3.h>
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