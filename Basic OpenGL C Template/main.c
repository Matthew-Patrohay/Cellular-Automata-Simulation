#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>


//--------- Global Variables ---------
int SIMULATION_WIDTH = 500; // Width of Game Window
int SIMULATION_HEIGHT = 500; // Height of Game Window

//--------- Structures ---------
// A single point for triangle generation
typedef struct triangle_point
{
    float x_value;
    float y_value;
    float z_value;
} triangle_point;

// The defining color for a pixel
typedef struct rgb_color_value
{
    float red;
    float green;
    float blue;
} rgb_color_value;

// The element of the pixel
typedef enum pixel_element
{
    WATER = 'w',
    AIR = '#',
    WALL = 'x',
    STONE = 's'
} pixel_element;

// The velocity Data for a pixel
typedef struct velocity_vector
{
    float x;
    float y;
} velocity_vector;

// All the data stored in a pixel
typedef struct simulation_pixel
{
    bool updated_this_cycle;
    velocity_vector velocity;
    pixel_element element;
    rgb_color_value color;
} simulation_pixel;

//--------- Declare functions ---------
void makeSquare(float x_cord, float y_cord, float z_cord, float red_value, float green_value, float blue_value, float width);

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// ---------- MAIN FUNCTION ----------
int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SIMULATION_WIDTH, SIMULATION_HEIGHT, "Cellular Automata", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // ---------- Graphics Loop ----------
    while (!glfwWindowShouldClose(window))
    {
        // Maintain aspect ratio when rendering
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1, 1, 1, -1);
        
        makeSquare(0, 0, 0, 1, 1, 1, 2);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void makeSquare(float x_cord, float y_cord, float z_cord, float red_value, float green_value, float blue_value, float width)
{
    float half_width = width / 2;
    // Point A Generation
    triangle_point point_A;
    point_A.x_value = x_cord - half_width;
    point_A.y_value = y_cord - half_width;
    point_A.z_value = 0;
    
    // Point B Generation
    triangle_point point_B;
    point_B.x_value = x_cord - half_width;
    point_B.y_value = y_cord + half_width;
    point_B.z_value = 0;
    
    // Point C Generation
    triangle_point point_C;
    point_C.x_value = x_cord + half_width;
    point_C.y_value = y_cord - half_width;
    point_C.z_value = 0;
    
    // Point D Generation
    triangle_point point_D;
    point_D.x_value = x_cord + half_width;
    point_D.y_value = y_cord + half_width;
    point_D.z_value = 0;
    
    // Generate a triangle strip which connects four vertices
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(red_value, green_value, blue_value);
    glVertex3f(point_A.x_value, point_A.y_value, point_A.z_value);
    glVertex3f(point_B.x_value, point_B.y_value, point_B.z_value);
    glVertex3f(point_C.x_value, point_C.y_value, point_C.z_value);
    glVertex3f(point_D.x_value, point_D.y_value, point_D.z_value);
    glEnd();
}
