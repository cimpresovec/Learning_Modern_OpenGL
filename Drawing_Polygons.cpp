//Libs
#ifdef _WIN32
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#endif // _WIN32

//Includes
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <fstream>
#include <sstream>
#include <iostream>

int main(int argc, char* args[])
{
    //Init stuff
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Polygons", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    glewInit();

    //Polygon stuff
    float vertices[] =
    {
        0.0f, 0.5f,
        0.3f, -0.5f,
        -0.3f, -0.5f    
    };

    GLuint vbo; //vertex buffer array
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Shader stuff
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    //load the stuff
    {
        std::ifstream shader_file("Shaders/polygons.vec");
        std::stringstream file_content;
        if (shader_file)
        {
            file_content << shader_file.rdbuf();
        }
        shader_file.close();
        std::string cont = file_content.str();
        const char* vertex_source = cont.c_str();
        glShaderSource(vertex_shader, 1, &vertex_source, NULL);
        glCompileShader(vertex_shader);
    }
    /*{
        std::ifstream shader_file("Shaders/polygons.frag");
        std::stringstream file_content;
        if (shader_file)
        {
            file_content << shader_file.rdbuf();
        }
        shader_file.close();
        const char* fragment_source = file_content.str().c_str();
        glShaderSource(fragment_shader, 1, &fragment_source, NULL);
        glCompileShader(fragment_shader);
    }*/

    int status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if (status == false)
    {
        char buffer[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, buffer);
        std::cout << buffer << "\n";
    }

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
