#define GLEW_STATIC 1  
#include <GL/glew.h>  

#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <vector>

#include "common.h"

//struct Particle {
//    glm::vec2 Position, Velocity;
//    glm::vec4 Color;
//    float     Life;
//
//    Particle()
//        : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
//};

//	May Ang
//	mang@ucsc.edu
//	CS 161 - Animation & Visualization
//	Final Project - Rain/Hail/Snow Simulation

//source: https://classes.soe.ucsc.edu/cmps161/Winter09/projects/mang/finalproject.html
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
float hailsize = 0.1;

int loop;


typedef struct {
    // Life
    bool alive;	// is the particle alive?
    float life;	// particle lifespan
    float fade; // decay
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

    par_sys[i].red = 1.0f;
    par_sys[i].green = 0.0f;
    par_sys[i].blue = 0.0f;

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
            y = par_sys[loop].ypos;
            z = par_sys[loop].zpos + zoom;

            glm::mat4 rainMatrix = worldMatrix * glm::scale(iMat, glm::vec3(2.0f, 1.0f, 2.0f));

            // Draw particles
            glBindVertexArray(cubeVao);
            setWorldMatrix(sceneShaderProgram, rainMatrix);
            setUniqueColor(sceneShaderProgram, par_sys[loop].red, par_sys[loop].blue, par_sys[loop].green);
            glBegin(GL_LINES);
            glVertex3f(x, y, z);
            glVertex3f(x, y + 160.0f, z);
            glColor3f(par_sys[loop].red, par_sys[loop].blue, par_sys[loop].green);
            glEnd();
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

// For Hail
void drawHail() 
{
    float x, y, z;

    for (loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
        if (par_sys[loop].alive == true) {
            x = par_sys[loop].xpos;
            y = par_sys[loop].ypos;
            z = par_sys[loop].zpos + zoom;

            // Draw particles
            glColor3f(0.8, 0.8, 0.9);
            glBegin(GL_QUADS);
            // Front
            glVertex3f(x - hailsize, y - hailsize, z + hailsize); // lower left
            glVertex3f(x - hailsize, y + hailsize, z + hailsize); // upper left
            glVertex3f(x + hailsize, y + hailsize, z + hailsize); // upper right
            glVertex3f(x + hailsize, y - hailsize, z + hailsize); // lower left
            //Left
            glVertex3f(x - hailsize, y - hailsize, z + hailsize);
            glVertex3f(x - hailsize, y - hailsize, z - hailsize);
            glVertex3f(x - hailsize, y + hailsize, z - hailsize);
            glVertex3f(x - hailsize, y + hailsize, z + hailsize);
            // Back
            glVertex3f(x - hailsize, y - hailsize, z - hailsize);
            glVertex3f(x - hailsize, y + hailsize, z - hailsize);
            glVertex3f(x + hailsize, y + hailsize, z - hailsize);
            glVertex3f(x + hailsize, y - hailsize, z - hailsize);
            //Right
            glVertex3f(x + hailsize, y + hailsize, z + hailsize);
            glVertex3f(x + hailsize, y + hailsize, z - hailsize);
            glVertex3f(x + hailsize, y - hailsize, z - hailsize);
            glVertex3f(x + hailsize, y - hailsize, z + hailsize);
            //Top
            glVertex3f(x - hailsize, y + hailsize, z + hailsize);
            glVertex3f(x - hailsize, y + hailsize, z - hailsize);
            glVertex3f(x + hailsize, y + hailsize, z - hailsize);
            glVertex3f(x + hailsize, y + hailsize, z + hailsize);
            //Bottom
            glVertex3f(x - hailsize, y - hailsize, z + hailsize);
            glVertex3f(x - hailsize, y - hailsize, z - hailsize);
            glVertex3f(x + hailsize, y - hailsize, z - hailsize);
            glVertex3f(x + hailsize, y - hailsize, z + hailsize);
            glEnd();

            // Update values
            //Move
            if (par_sys[loop].ypos <= -10) {
                par_sys[loop].vel = par_sys[loop].vel * -1.0;
            }
            par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 1000); // * 1000
            par_sys[loop].vel += par_sys[loop].gravity;

            // Decay
            par_sys[loop].life -= par_sys[loop].fade;

            //Revive
            if (par_sys[loop].life < 0.0) {
                initParticles(loop);
            }
        }
    }
}

// For Snow
void drawSnow(glm::mat4 worldMatrix, int sphereVao, int sceneShaderProgram, std::vector<int> snowIndices, GLuint snowTextureID)
{
    float x, y, z;
    for (loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
        if (par_sys[loop].alive == true) {
            x = par_sys[loop].xpos;
            y = par_sys[loop].ypos;
            z = par_sys[loop].zpos + zoom;

            // Draw particles
            glColor3f(1.0, 1.0, 1.0);
            glPushMatrix();
            glTranslatef(x, y, z);
            drawSphere(worldMatrix, sphereVao, sceneShaderProgram, snowIndices, snowTextureID);
            glPopMatrix();

            // Update values
            //Move
            par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 1000);
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

