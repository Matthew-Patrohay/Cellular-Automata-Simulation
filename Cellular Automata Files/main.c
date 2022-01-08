#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "Header.h"
#include "Functions.h"

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