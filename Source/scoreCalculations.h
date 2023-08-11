#define GLEW_STATIC 1  
#include <GL/glew.h>  

#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include "common.h"

int score1[] = {0, 0};
int score2[] = {0, 0};
int setScore1 = 0;
int setScore2 = 0;



// IMPORTANT: numbers 11 and 12 represents A and d respectively***
void score(bool p1Scored, bool p2Scored) 
{
	if (p1Scored) 
	{
		switch (score1[0])
		{
		case 0:
			score1[0] = 1;
			score1[1] = 5;
			break;
		case 1:
			score1[0] = 3;
			score1[1] = 0;
			break;
		case 3:
			score1[0] = 4;
			score1[1] = 0;
			break;
		case 4:
			if (score2[0] == 4)
			{
				score1[0] = 11;
				score1[1] = 12;
			}
			else if (score2[0] == 11)
			{
				score2[0] = 4;
				score2[1] = 0;
			}
			else
			{
				score1[0] = 0;
				score1[1] = 0;
				score2[0] = 0;
				score2[1] = 0;
				setScore1++;
				SoundEngine->play2D(cheerSource, false);
				isP1sTurnToServe = !isP1sTurnToServe;
			}
			break;
		case 11:
			score1[0] = 0;
			score1[1] = 0;
			score2[0] = 0;
			score2[1] = 0;
			setScore1++;
			SoundEngine->play2D(cheerSource, false);
			isP1sTurnToServe = !isP1sTurnToServe;
			break;
		}
	}

	if (p2Scored)
	{
		switch (score2[0]) 
		{
		case 0:
			score2[0] = 1;
			score2[1] = 5;
			break;
		case 1:
			score2[0] = 3;
			score2[1] = 0;
			break;
		case 3:
			score2[0] = 4;
			score2[1] = 0;
			break;
		case 4:
			if (score1[0] == 4)
			{
				score2[0] = 11;
				score2[1] = 12;
			}
			else if (score1[0] == 11)
			{
				score1[0] = 4;
				score1[1] = 0;
			}
			else
			{
				score1[0] = 0;
				score1[1] = 0;
				score2[0] = 0;
				score2[1] = 0;
				setScore2++;
				SoundEngine->play2D(cheerSource, false);
				isP1sTurnToServe = !isP1sTurnToServe;
			}
			break;
		case 11:
			score1[0] = 0;
			score1[1] = 0;
			score2[0] = 0;
			score2[1] = 0;
			setScore2++;
			SoundEngine->play2D(cheerSource, false);
			isP1sTurnToServe = !isP1sTurnToServe;
			break;
		}
	}

	if (!isSimulation) {
		if (setScore2 == 7) {
			resetScore();
			SoundEngine->play2D(gameOverSource, false);
		}
		else if (setScore1 == 7) {
			resetScore();
			SoundEngine->play2D(winSource, false);
		}
	}
	else {
		if (setScore1 == 7 || setScore2 == 7) {
			resetScore();
			SoundEngine->play2D(gameOverSimSource, false);
		}
	}
}

void resetScore() {
	score1[0] = 0;
	score1[1] = 0;
	score2[0] = 0;
	score2[1] = 0;
	setScore1 = 0;
	setScore2 = 0;
}