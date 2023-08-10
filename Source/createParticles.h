#define GLEW_STATIC 1  
#include <GL/glew.h>  

#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <vector>

#include "common.h"

//Inspired by: https://classes.soe.ucsc.edu/cmps161/Winter09/projects/mang/finalproject.html
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define MAX_PARTICLES 500
#define WCX		640
#define WCY		480


float slowdown = 2.0;
float velocity = 0.0;
float zoom = -60.0;
float pan = 0.0;
float tilt = 0.0;
int loop;


typedef struct {
    // Life
    bool alive;	
    float life;	
    float fade; 
    // color
    float red;
    float green;
    float blue;
    // Position/direction
    float xpos;
    float ypos;
    float zpos;
    // Velocity/Direction, only goes down in y dir
    float vel;
    // Gravity
    float gravity;
}particles;

// Paticle System
particles par_sys[MAX_PARTICLES];

// Initialize/Reset Particles - give them their attributes
void initParticles(int i) {
    par_sys[i].alive = true;
    par_sys[i].life = 1.0;
    par_sys[i].fade = float(rand() % 100) / 1000.0f + 0.003f;

    par_sys[i].xpos = (float)(rand() % 300) - 150;
    par_sys[i].ypos = -10.0f;
    par_sys[i].zpos = (float)(rand() % 300) - 150;

    par_sys[i].red = 0.0f;
    par_sys[i].green = .0f;
    par_sys[i].blue = 1.0f;

    par_sys[i].vel = velocity;
    par_sys[i].gravity = -0.8;

}

// For Rain
void drawRain(glm::mat4 worldMatrix, int sceneShaderProgram, int cubeVao) 
{
    float x, y, z;
    for (loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
        if (par_sys[loop].alive == true) {
            x = par_sys[loop].xpos;
            y = par_sys[loop].ypos + 50.0f;
            z = par_sys[loop].zpos;

            glm::mat4 rainMatrix = worldMatrix * glm::translate(iMat, glm::vec3(x, y, z)) * glm::scale(iMat, glm::vec3(0.03f, 300.0f, 0.03f));

            // Draw particles
            glBindVertexArray(cubeVao);
            setWorldMatrix(sceneShaderProgram, rainMatrix);
            setUniqueColor(sceneShaderProgram, 0.0f, 0.489f, 1.0f);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);

            // Update values
            //Move
            // Adjust slowdown for speed!
            par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 1000);
            par_sys[loop].vel += par_sys[loop].gravity;
            // Decay
            par_sys[loop].life -= par_sys[loop].fade;

            if (par_sys[loop].ypos <= -20) {
                par_sys[loop].life = -1.0;
                //par_sys[loop].alive = false;
            }
            //Revive
            if (par_sys[loop].life < 0.0) {
                initParticles(loop);
            }
        }
    }
}

// For Snow
void drawSnow(glm::mat4 worldMatrix, int sceneShaderProgram, int sphereVao, std::vector<int> indices)
{
    float x, y, z;
    for (loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
        if (par_sys[loop].alive == true) {
            x = par_sys[loop].xpos;
            y = par_sys[loop].ypos + 50.0f;
            z = par_sys[loop].zpos;

            glm::mat4 snowMatrix = worldMatrix * glm::translate(iMat, glm::vec3(x, y, z)) * glm::scale(iMat, glm::vec3(0.3f, 0.3f, 0.3f));

            // Draw particles
            glBindVertexArray(sphereVao);
            setWorldMatrix(sceneShaderProgram, snowMatrix);
            setUniqueColor(sceneShaderProgram, 1.0f, 0.0f, 0.0f);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Update values
            //Move
            par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 100000);
            par_sys[loop].vel += par_sys[loop].gravity;
            // Decay
            par_sys[loop].life -= par_sys[loop].fade;

            if (par_sys[loop].ypos <= -20) {
                par_sys[loop].life = -1.0;
            }

            //Revive
            if (par_sys[loop].life < 0.0) {
                initParticles(loop);
            }
        }
    }
}

