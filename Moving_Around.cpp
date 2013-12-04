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
    Player(const int shader);

    bool up_button, down_button, left_button, right_button;
    float x, y, w, h;

    void handleInput(GLFWwindow* main_window);
    void doLogic();
    void render();

private:
    GLuint vbo;
    GLuint vao;
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
    glClearColor(0.f, 0.f, 0.f, 1.f);

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Game objects
    Player* player_one = new Player(shader_program);
    Player* player_two = new Player(shader_program);

    //Main game loop
    while (!glfwWindowShouldClose(main_window))
    {
        //EVENTS
        glfwPollEvents();
        if (glfwGetKey(main_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(main_window, true);
        }
        player_one->handleInput(main_window);

        //LOGIC
        player_one->doLogic();

        //RENDERING
        glClear(GL_COLOR_BUFFER_BIT);

        
        //Render
        player_one->render();
        player_two->render();

        glfwSwapBuffers(main_window);
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); //Delay
    }

    return 0;
}

Player::Player(const int shader)
{
    left_button = right_button = up_button = down_button = false;
    x = y = 0.f;
    w = h = .2f;

    //vbo
    float vertices[] = 
    {
        x-w/2, y-h/2, 1.f, 1.f, 1.f, 1.f,
        x+w/2, y-h/2, 0.f, 0.f, 1.f, 1.f,
        x+w/2, y+h/2, 1.f, 0.f, 0.f, 1.f,
        x-w/2, y+h/2, 0.f, 1.f, 0.f, 1.f
    };
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
    
    //vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Attributes
    GLint position_attribute = glGetAttribLocation(shader, "in_position");
    glEnableVertexAttribArray(position_attribute);
    glVertexAttribPointer(position_attribute, 2, GL_FLOAT, 0, 6*sizeof(float), 0);

    GLint color_attribute = glGetAttribLocation(shader,"in_color");
    glEnableVertexAttribArray(color_attribute);
    glVertexAttribPointer(color_attribute, 4, GL_FLOAT, 0, 6*sizeof(float), (void*)(2*sizeof(float)));
    glBindVertexArray(0);
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
    glm::vec2 speed(0.f, 0.f);
    if (right_button)
    {
        speed.x += 0.001f;
    }
    if (left_button)
    {
        speed.x -= 0.001f;
    }
    if (up_button)
    {
        speed.y += 0.001f;
    }
    if (down_button)
    {
        speed.y -= 0.001f;
    }

    x += speed.x;
    y += speed.y;
}  

void Player::render()
{
    //vbo
    //update vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, 0, NULL);
    
    float vertices[] = 
    {
        x-w/2, y-h/2, 1.f, 1.f, 1.f, .5f,
        x+w/2, y-h/2, 0.f, 0.f, 1.f, .5f,
        x+w/2, y+h/2, 1.f, 0.f, 0.f, .5f,
        x-w/2, y+h/2, 0.f, 1.f, 0.f, .5f
    };

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    //Render
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
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