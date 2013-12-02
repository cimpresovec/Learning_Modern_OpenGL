//Libs
#ifdef _WIN32
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "SOIL.lib")
#endif

//Headers
#ifdef _WIN32
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

//Default headers
#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>

//Compiling shader TODO make better
GLuint compileShader(const std::string file_name, int shader_type, bool print_error = true);

struct Player
{
    Player();

    bool up_button, down_button, left_button, right_button;
    float x, y, w, h;

    GLuint vbo;

    void handleInput(GLFWwindow* main_window);
    void doLogic();
    void render();
};

int main(int argc, char* args[])
{
    //glfw initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);
    GLFWwindow* main_window = glfwCreateWindow(800, 600, "Moving around", nullptr, nullptr);
    glfwMakeContextCurrent(main_window);
    glClearColor(.5f, .5f, .5f, 1.f);

    //glew initialization
    glewExperimental = true;
    glewInit();

    //Load shaders
    GLuint vertex_shader = compileShader("Shaders/moving_around.ver", GL_VERTEX_SHADER);
    GLuint fragment_shader = compileShader("Shaders/moving_around.frag", GL_FRAGMENT_SHADER);

    //Shader program
    GLint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glUseProgram(shader_program);

    //Vertex array object
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Shader attributes
    

    //Game objects
    //Player* player_one = new Player();

    

    //Main game loop
    while (!glfwWindowShouldClose(main_window))
    {
        //EVENTS
        glfwPollEvents();
        if (glfwGetKey(main_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(main_window, true);
        }
        //player_one->handleInput(main_window);

        //LOGIC
        //player_one->doLogic();

        //RENDERING
        glClear(GL_COLOR_BUFFER_BIT);

        
        //Render
        float vertices[] =
        {
            -.5f, .5f,  1.f, 1.f, 1.f, 1.f,
                .5f, .5f,   1.f, 1.f, 1.f, 1.f,
                .5f, -.5f,  1.f, 1.f, 1.f, 1.f,
                -.5f, -.5f, 1.f, 1.f, 1.f, 1.f
        };

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        GLint position_attribute = glGetAttribLocation(shader_program, "in_position");
        glEnableVertexAttribArray(position_attribute);
        glVertexAttribPointer(position_attribute, 2, GL_FLOAT, 0, 6*sizeof(float), 0);

        GLint color_attribute = glGetAttribLocation(shader_program,"in_color");
        glEnableVertexAttribArray(color_attribute);
        glVertexAttribPointer(color_attribute, 4, GL_FLOAT, 0, 6*sizeof(float), (void*)(2*sizeof(float)));

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        //player_one->render();

        glfwSwapBuffers(main_window);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); //Delay
    }

    return 0;
}

Player::Player()
{
    left_button = right_button = up_button = down_button = false;
    x = y = 0.f;
    w = h = .2f;

    //vbo
    /*
    float vertices[] = 
    {
        x-w/2, y-h/2, 1.f, 1.f, 1.f, 1.f,
        x+w/2, y-h/2, 0.f, 0.f, 1.f, 1.f,
        x+w/2, y+h/2, 1.f, 0.f, 0.f, 1.f,
        x-w/2, y+h/2, 0.f, 1.f, 0.f, 1.f
    };
    */
    
}

void Player::handleInput(GLFWwindow* main_window)
{
    if (glfwGetKey(main_window, GLFW_KEY_W) == GLFW_PRESS) up_button = true;
    else up_button = false;
    if (glfwGetKey(main_window, GLFW_KEY_S) == GLFW_PRESS) down_button = true;
    else down_button = false;
    if (glfwGetKey(main_window, GLFW_KEY_A) == GLFW_PRESS) left_button = true;
    else left_button = false;
    if (glfwGetKey(main_window, GLFW_KEY_D) == GLFW_PRESS) right_button = true;
    else right_button = false;
}

void Player::doLogic()
{

}  

void Player::render()
{
    //vbo
    
    //update vbo
    //glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), NULL);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    float vertices[] =
    {
        -.5f, .5f,  1.f, 1.f, 1.f, 1.f,
        .5f, .5f,   1.f, 1.f, 1.f, 1.f,
        .5f, -.5f,  1.f, 1.f, 1.f, 1.f,
        -.5f, -.5f, 1.f, 1.f, 1.f, 1.f
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    //Render
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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