//libs
#ifdef _WIN32
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "SOIL.lib")
#endif

//Includes
#ifdef _WIN32
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

//Messy unoptimized function to compile a shader and outputing errors
GLuint compileShader(const std::string file_name, int shader_type, bool print_error = true);

int main(int argc, char* args[])
{
    //glfw stuff
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Transformations", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glClearColor(1.f, 1.f, 1.f, 1.f);

    //glew stuff
    glewExperimental = true;
    glewInit();

    //vertices x, y, r, g, b, tex_x, tex_y
    float vertices[] =
    {
        -.5f, .5f,  1.f, 1.f, 1.f,  0.f, 0.f,
        .5f, .5f,   1.f, 1.f, 1.f,  1.f, 0.f,
        .5f, -.5f,  1.f, 1.f, 1.f,  1.f, 1.f,
        -.5f, -.5f, 1.f, 1.f, 1.f,  0.f, 1.f
    };

    //Vertex buffer array
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Vertex array object
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    //Element buffer object
    GLuint element_buffer;
    glGenBuffers(1, &element_buffer);
    
    int elements[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    //Compile shaders
    GLuint vertex_shader = compileShader("Shaders/transformations.vec", GL_VERTEX_SHADER);
    GLuint fragment_shader = compileShader("Shaders/transformations.frag", GL_FRAGMENT_SHADER);

    //Create program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glUseProgram(shader_program);

    //Shader attributes
    GLint position_attribute = glGetAttribLocation(shader_program, "in_position");
    glEnableVertexAttribArray(position_attribute);
    glVertexAttribPointer(position_attribute, 2, GL_FLOAT, 0, 7*sizeof(float), 0);
    
    GLint color_attribute = glGetAttribLocation(shader_program, "in_color");
    glEnableVertexAttribArray(color_attribute);
    glVertexAttribPointer(color_attribute, 3, GL_FLOAT, 0, 7*sizeof(float), (void*)(2*sizeof(float)));

    GLint tex_attribute = glGetAttribLocation(shader_program, "in_tex_coord");
    glEnableVertexAttribArray(tex_attribute);
    glVertexAttribPointer(tex_attribute, 2, GL_FLOAT, 0, 7*sizeof(float), (void*)(5*sizeof(float)));

    //Load texture
    GLuint basic_texture;
    glGenTextures(1, &basic_texture);
    glBindTexture(GL_TEXTURE_2D, basic_texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width = 0, height = 0;
    unsigned char* soil_image = SOIL_load_image("Data/img_test.png", &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, soil_image);
    SOIL_free_image_data(soil_image);

    //Blending - alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Transformations
    GLint uni_model = glGetUniformLocation(shader_program, "model");
    float rotate = 0.f;
    float player_x = 0.f, player_y = -1.f;
        
    //View and projection transformations
    GLint view_uniform = glGetUniformLocation(shader_program, "view");
    
    
    glm::mat4 proj_matrix = glm::perspective(100.f, 800.f/600.f, 0.f, 10.f);
    GLint proj_uniform = glGetUniformLocation(shader_program, "proj");
    glUniformMatrix4fv(proj_uniform, 1, GL_FALSE, glm::value_ptr(proj_matrix));
    
    while(!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) rotate += .1f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rotate -= .1f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player_y += .001f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) player_y -= .001f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) player_x += .001f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) player_x -= .001f;

        //Transformations
        //Rotations
        glm::mat4 transformation;
        transformation = glm::translate(transformation, glm::vec3(player_x, player_y, 0.f));
        transformation = glm::rotate(transformation, rotate, glm::vec3(1.f, 0.f, 0.f));
        //transformation = glm::translate(transformation, glm::vec3(.5f, 0.f, 0.f));
        transformation = glm::rotate(transformation, rotate, glm::vec3(0.f, 0.f, 1.f));
        //transformation = glm::translate(transformation, glm::vec3(-.5f, 0.f, 0.f));
        
        //Camera
        glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.f, -1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
        
        glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view_matrix));
        //Apply transformation
        
        glUniformMatrix4fv(uni_model, 1, GL_FALSE, glm::value_ptr(transformation));

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); //Some wait so we don't eat the CPU
    }

    return 0;
}

GLuint compileShader(const std::string file_name, int shader_type, bool print_error)
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