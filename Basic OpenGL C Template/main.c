#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "Header.h"

// ---------- MAIN FUNCTION ----------
int main(void)
{
    // Create Window
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);

    // Start glfw
    if (!glfwInit())
        return -1;

    // Create Window
    window = glfwCreateWindow(SIMULATION_WIDTH, SIMULATION_HEIGHT, "Cellular Automata", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make window context current
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // Generate an array for the simulation pixels
    simulation_pixel pixel_array[(int)SIMULATION_GRID_RESOLUTION][(int)SIMULATION_GRID_RESOLUTION];
    for (int i = 0; i < SIMULATION_GRID_RESOLUTION; i++)
    {
        for (int j = 0; j < SIMULATION_GRID_RESOLUTION; j++)
        {
            //Set all current pixels to air
            pixel_array[i][j].element = AIR;
            
            //Set a few random pixels to water for testing
            if (randomZeroOne() > 0.9)
            {
                pixel_array[i][j].element = WATER;
            }
            
            //Set the border to walls
            if (i <= 1 || j <= 1 || i >= (SIMULATION_GRID_RESOLUTION - 2) || j >= (SIMULATION_GRID_RESOLUTION - 2))
            {
                pixel_array[i][j].element = WALL;
            }
        }
    }
    
    // ---------- Main Graphics Loop ----------
    while (!glfwWindowShouldClose(window))
    {
        // Maintain aspect ratio when rendering
        float ratio;
        int width;
        int height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1, 1, 1, -1);
        
        // Swap front and back buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Close window and end glfw
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
static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
float randomZeroOne(void)
{
    return((float) rand() / RAND_MAX);
}
