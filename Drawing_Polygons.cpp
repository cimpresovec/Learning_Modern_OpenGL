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

    //Polygon stuff, X Y R G B
    float vertices[] =
    {
        0.0f, 0.5f, 0.5f, 1.0f, 0.f,
        0.3f, -0.5f, 0.f, 0.f, 1.f,
        -0.3f, -0.5f, 1.f, 1.f, 0.5f,
        -.5f, .5f, 1.f, 0.f, 1.f
    };

    GLuint vbo; //vertex buffer array
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Element buffer
    GLuint elements[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    GLuint ebo;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    //Shader stuff
    GLuint vertex_shader = compileShader("Shaders/polygons.vec", GL_VERTEX_SHADER);
    GLuint fragment_shader = compileShader("Shaders/polygons.frag", GL_FRAGMENT_SHADER);

    //Shader program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glUseProgram(shader_program);

    //Vertex array buffer
    GLuint vao;
    glGenBuffers(1, &vao);
    glBindVertexArray(vao);

    //Vertex attributes
    GLint position_attribute = glGetAttribLocation(shader_program, "position");
    glVertexAttribPointer(position_attribute, 2, GL_FLOAT, false, 5*sizeof(float), 0);
    glEnableVertexAttribArray(position_attribute);

    GLint color_attribute = glGetAttribLocation(shader_program, "in_colors");
    glVertexAttribPointer(color_attribute, 3, GL_FLOAT, false, 5*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(color_attribute);

    //Uniform alpha multiply
    GLint uni_red = glGetUniformLocation(shader_program, "uni_red");
    float change = 1.f;

    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
        
        //Fast fading stuff
        glUniform1f(uni_red, (float)sin(change += .001f));

        //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        //or
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    return 0;
}
