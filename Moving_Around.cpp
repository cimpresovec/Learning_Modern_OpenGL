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
    const float k_x, k_y;

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);
    GLFWwindow* main_window = glfwCreateWindow(800, 600, "Moving around", nullptr, nullptr);
    glfwMakeContextCurrent(main_window);
    glClearColor(0.f, 0.f, 0.f, 1.f);

    //glew initialization
    glewExperimental = true;
    glewInit();

    //Vertex array object
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Game objects
    Player* player_one = new Player();

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

        glfwSwapBuffers(main_window);
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); //Delay
    }

    return 0;
}

Player::Player() : k_x(0.f), k_y(0.f)
{
    left_button = right_button = up_button = down_button = false;
    x = y = k_x;
    w = h = .2f;

    //vbo
    float vertices[] = 
    {
        x-w/2, y-h/2,
        x+w/2, y-h/2,
        x+w/2, y+h/2,
        x-w/2, y+h/2
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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

}