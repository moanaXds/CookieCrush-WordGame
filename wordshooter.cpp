//============================================================================
// Name        : cookie-crush.cpp
// Author      : Muanna Hamid
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Cookie  Crush...
//============================================================================


#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP

//#include <GL/gl.h>
//#include <GL/glut.h>

#include <iostream>
#include<string>
#include<cmath>
#include<fstream>
#include<ctime>
#include<cstdlib>
#include "util.h"

using namespace std;

#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 30

string * dictionary;

#define KEY_ESC 27 

//--------------        Necessary Variables        -------------


int glocookiey=0;
int dictionarysize = 370099;

const int bradius = 30; // cookie radius in pixels...

const float maxTime = 120.0f; 

int arrayer[5]={19,0,18,18,10};

int width = 930, height = 660;
int byoffset = bradius;

int nxcells = (width-bradius) / (2*bradius);
int nycells = (height-byoffset) / (2*bradius);

int nfrows = 2; // initially number of full rows 
float score = 0;
int **board; // 2D-arrays for holding the data...
int bwidth = 130;
int bheight = 10;
int bsx, bsy;

//Can you make a timer of 120 seconds and display it on the screen

int elapsedTime = 0; // Elapsed time in seconds
bool gameOver = false; // Game over flag
const int gameDuration = 120; 

const int nalphabets = 26;
float cookieSpeedX = 0;  // X speed of the cookie
float cookieSpeedY = 0;
int cookieX = width / 2;  // Initial cookie clposition (centered)
int cookieY = 0;          // cookie is at the bottom initially
float cookieSpeed = 30.0f;
bool cookieLaunched = false;  // cookie is not launched yet 
float timeElapsed = 0.0f;  // Time in seconds


enum alphabets 
{
	AL_A, AL_B, AL_C, AL_D, AL_E, AL_F, AL_G, AL_H, AL_I, AL_J, AL_K, AL_L, AL_M, AL_N, AL_O, AL_P, AL_Q, AL_R, AL_S, AL_T, AL_U, AL_W, AL_X, AL_y, AL_Z
};


GLuint texture[nalphabets];
GLuint tid[nalphabets];

string tnames[] = {"a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
		     "k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
		     "x.bmp", "y.bmp", "z.bmp" };

GLuint mtid[nalphabets];
int awidth = 60, aheight = 60; // 60x60 pixels cookies...



//--------------------------------------------       Game Board Function        --------------------------------------------------


//USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE
void RegisterTextures_Write()
//Function is used to load the textures from the
// files and display
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each cookies data...

	for (int i = 0; i < nalphabets; ++i) {

		// Read current cookie

		ReadImage(tnames[i], data);
		if (i == 0) {
			int length = data.size();
			ofile.write((char*)&length, sizeof(int));
		}
		ofile.write((char*)&data[0], sizeof(char) * data.size());

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}

	ofile.close();

}


void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile) {
		cout << " Couldn't Read the Image Data file ";
		//exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i) {
		// Read current cookie
		//ReadImage(tnames[i], data);
		/*if (i == 0) {
		int length = data.size();
		ofile.write((char*) &length, sizeof(int));
		}*/
		ifile.read((char*)&data[0], sizeof(char)* length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}


void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
	int cheight = 60)
	/*Draws a specfic cookie at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed cookie in pixels
	* cheight= height of displayed cookiei pixels.
	* */
{
	float fwidth = (float)cwidth / width * 2; 
	float fheight = (float)cheight / height * 2;

	float fx=(float)sx / width * 2 - 1;
	float fy=(float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();
	

	glColor4f(1, 1, 1, 1);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

void Drawcookie(const alphabets &cname, int sx, int sy, int cwidth = 60,
	int cheight = 60)
	/*Draws a specfic cookie at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed cookie in pixels
	* cheight= height of displayed cookiei pixels.
	* */
{
	float fwidth = (float)cwidth / width * 2;
	float fheight = (float)cheight / height * 2;
	float fx = (float)sx / width * 2 - 1;
	float fy = (float)sy / height * 2 - 1;

	fx=fx-1;
	fy=fy-1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}
int GetAlphabet() {
	return GetRandInRange(1, 26);
}



void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)

{
	float fwidth = (float)cwidth / width * 2;
	float fheight = (float)cheight/height * 2;
	float fx = (float)sx / width * 2 - 1;
	float fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, -1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();
	

	glColor4f(1, 1, 1, 1);


	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}


//======================================= Implentation for Search And Burst  =======================

int cookie;
int alphabetGrid[5][15]; 

bool isLetter(const string dict[], int size, const string& word)
{
	int i = 0;

	while (i<size) 
	{
		if (dict[i]==word) 
		{
			return true;
		}
		++i; 
	}
	return false; 
}




bool SearchForMatchedWords(int grid[5][15], const string dictionary[], int dictionarySize, string& foundWords) 
{
const int path[6][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1} };

foundWords = ""; // Clear the foundWords string

for (int w=0 ; w<dictionarySize ; w++) 
{
	string word = dictionary[w];
	int LenghtWord = word.length();

	// Skip words that are too short or long (Base case)
	if (LenghtWord<3 || LenghtWord>15) 
	{
		continue;
	}

	// Iterate through each cell in the grid
	for(int col=0; col<5; col++) 
	{
		for(int row=0; row < 15; row++) 
		{
			for(int p=0; p< 6; p++) 
			{
				int allcol = path[p][0];
				int allrow = path[p][1];

				int finalcol = col + (LenghtWord-1)*allcol;
				int finalrow = row + (LenghtWord-1)*allrow;

				if (finalrow<0 || finalcol>= 5 || finalcol<0 || finalrow>=15) 
				{
					continue; // Word does not fit
				}

				bool match = true;

				// Check each character in the word
				for (int k = 0; k < LenghtWord; k++) 
				{
					int colpresent = col+k*allcol;
					int rowpresent = row+k*allrow;

					int NumGrid = grid[colpresent][rowpresent];
					char gridAlpha = 'a'+NumGrid; // Convert grid value to character

					if (gridAlpha != word[k]) 
					{
						match = false;
						break; // Mismatch
					}
				}

				// If match is found
				if (match) 
				{
					// Add the word to the foundWords string, separated by spaces
					if (!foundWords.empty()) 
					{
						foundWords = foundWords+"   ";
					}

					foundWords = foundWords+word;

					// Mark the grid cells as used
					for(int m=0; m < LenghtWord ; m++) 
					{
						int colpresent = col+m*allcol;
						int rowpresent = row+m*allrow;
						grid[colpresent][rowpresent] = rand()%26; // Randomize the cell
					}

					return true; // Exit once a word is found
				}
			}
		}
	}
}

return false; // No word found
}


void SearchWordOnBoard( int grid[5][15], const string dictionary[], int dictionarySize) 
{
	string largestWord = ""; 
	int finalcol = 1;
	int finalrow= 1;     
	int Allcolumn_end = 0;
	int final_allrow = 0; 
	int first_col_index = 1;
	int first_row_index= 1; 

	const int path[6][2] = { {1, 0},  {-1, 0},  {0, 1},   {0, -1}, {1, 1},  {-1, -1}  };

	// Iterate through each word in the dictionary
	for(int w = 0; w < dictionarySize; ++w) 
	{
		string word = dictionary[w];
		int LenghtWord = word.length();

		if(LenghtWord<3 || LenghtWord>15) 
		{
			continue;
		}

		// Iterate through each cell in GRID
		for(int col = 0; col < 5; ++col)
		{
			for(int row = 0; row < 15; ++row) 
			{
				for(int d = 0; d < 6; ++d)
				{
					int allcol = path[d][0];
					int allrow = path[d][1];

					// Calculate word's ending position 
					int finalcol = col+(LenghtWord-1) * allcol;
					int finalrow= row+(LenghtWord-1) * allrow;

					if(finalcol<0 || finalcol>=5 || finalrow<0 || finalrow>=15) 
					{
						continue; 
					}

					bool match = true; // check 

					// Check each character in the word
					for(int k=0; k < LenghtWord; k++)
					{
						int colpresent = col + k * allcol;
						int rowpresent = row + k * allrow;

						int NumGrid = grid[colpresent][rowpresent];
						char gridAlpha = 'a' + NumGrid; 

						// Comparism of ...
						if(gridAlpha != word[k]) 
						{
							match = false;
							break; 
						}
					}

						// if a word matched to the alpha_txt file ,add it...................................
					if(match)
					{
						if (LenghtWord > largestWord.length() )// numeric value (size)
						{
							largestWord = word;
							first_col_index = col;
							first_row_index= row;
							finalcol = finalcol;
							finalrow= finalrow;
							Allcolumn_end = allcol;
							final_allrow = allrow;
						}
					}
				}
			}            
		}
	}

	if (!largestWord.empty())
	{
		int LenghtWord = largestWord.length();

		for (int m = 0; m < LenghtWord; ++m)
		{
			int colpresent = first_col_index + m * Allcolumn_end;
			int rowpresent = first_row_index+ m * final_allrow;
			grid[colpresent][rowpresent] = -1; //  used ALready
		}
	}	

	cout<<largestWord<<endl;
	score=score+largestWord.length()*10;
	// Return all Founded words
}




void GenerateAlphabetGrid() 
{
	string foundWords;

	do
	{
		int j=0; 
		while(j<5) 
		{ 
			if(j<2)
			{
				int i=0; 
				while(i<15) 
				{ 
					alphabetGrid[j][i] = rand() % 26; 
					++i; 
				}
			} 

			else 
			{
				int i=0; 
				while(i<15) 
				{ 
					alphabetGrid[j][i] = -1;  
					++i; 
				}
			}

			++j;
		}
	}
	while (SearchForMatchedWords(alphabetGrid, dictionary, dictionarysize, foundWords));
	cookie = rand() % 26;
}


void addToGrid(int alphabetGrid[5][15],int bx,int by)
{
	int x=(bx)/60;
	int y=(height-by-60)/60;
	alphabetGrid[y][x]=cookie;
	
	cout<<height<<' '<<y<<' '<<x<<' '<<by<<' '<<bx<<endl;
}


bool IsCollisionOccur(int cookieX, int cookieY, int radius, int rowY, int alphabetGrid[5][15],int r)
{
	for (int i = 0; i < 15; i++)
	{
		int xpositionAlpha= i * 60-40;  // x position  
		int ypositionAlpha = rowY;  // y position 

		if(alphabetGrid[r][i] == -1)
		{
			continue;
		}  

		if(cookieX + radius > xpositionAlpha && cookieX - radius < xpositionAlpha+ 60 && 
		   cookieY + radius  > ypositionAlpha && cookieY - radius < ypositionAlpha + 60) 
		{
			cout<<r<<i<<endl;

			return true;  // Collision 
		}
	}

	return false;  // No collision
}


void DisplayFunction()
{
	glClearColor(1.0, 1.0, 1.0, 0.0); // Set background to white
	glClear(GL_COLOR_BUFFER_BIT);

	if (gameOver)
	{
		glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
		DrawString(width / 2 - 80, height / 2, width, height, "GAME OVER", colors[BROWN]);
		DrawString(width / 2 - 90, height / 2 - 30, width, height, "Press ESC to exit", colors[BROWN]);
	}

	else
	{
		//First Upgrade Shooter cookie  positon in both direction
		cookieX += cookieSpeedX * cookieSpeed;
		cookieY += cookieSpeedY * cookieSpeed;

		/// don't go out of board

		if (cookieX > width - bradius) 
		{
			cookieX = width - bradius;  
			cookieSpeedX = -cookieSpeedX;  
		}

		// REFLECTION FROM WALLS

		if (cookieX < bradius - 50) 
		{
			cookieX = bradius;  // Prevent cookie from going out of bounds
			cookieSpeedX = -cookieSpeedX;  // Reflect the cookie horizontally
		}


		// FOR top and bottom walls
		if (cookieY < bradius) 
		{
			cookieY = bradius;  // Prevent cookie from going out of bounds
			cookieSpeedY = -cookieSpeedY;  // Reflect the cookie vertically
		}

		if (cookieY > height - bradius) 
		{
			cookieY = height - bradius;  // Prevent cookie from going out of bounds
			cookieSpeedY = -cookieSpeedY;  // Reflect the cookie vertically
		}

		// Check for collision with upper row _Pof alphabets
		for(int i=0;i<5;i++)
		{
			if(IsCollisionOccur(cookieX, cookieY, bradius, height - (i*60+140), alphabetGrid , i)) 
			{
				// Handle collision (e.g., destroy cookie and spawn a new one)
				addToGrid(alphabetGrid,cookieX,cookieY);
				SearchWordOnBoard(alphabetGrid,dictionary,dictionarysize);
				cookieX = width/2;  // Reset cookie position
				cookieY = 10;
				cookieSpeedX = 0;  // Reset cookie speed
				cookieSpeedY = 0;
				cookieLaunched = false;  // cookie is not launched
				cookie = arrayer[glocookiey%5];  // Get new alphabet cookie
				glocookiey=glocookiey+1;
			}
		}
		// Draw the cookie (alphabet) at the updated position

		// Draw the upper row of alphabets

		int gap=120;

		for(int j=0 ; j<5 ; j++)
		{
			int xGap = 10;

			for (int i = 0; i < 15; i++)
			{
				int curr_height=height-gap;
				DrawAlphabet((alphabets)alphabetGrid[j][i], xGap, height - gap, awidth, aheight); 
				xGap = xGap + 60;
			}
			gap=gap+60;
		}

		DrawAlphabet((alphabets)cookie, cookieX, cookieY, awidth, aheight);	

		// Draw other game elements (score, timer, etc.)
		DrawString(40, height - 20, width, height + 5, "Score " + Num2Str(score), colors[BLUE_VIOLET]);
		DrawString(width / 2 - 30, height - 25, width, height, "Time Left:" + Num2Str(gameDuration-elapsedTime) + " secs", colors[RED]);

		DrawShooter((width / 2) - 35, 0, bwidth, bheight);
	}
	glutSwapBuffers();
}



/* Function sets canvas size (drawing area) in pixels...
*  that is what dimensions (x and y) your game will have
*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
* */
void SetCanvasSize(int width, int height) {
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.*/
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
* is pressed from the keyboard
*
* You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
*
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
*
* */

void NonPrintableKeys(int key, int x, int y) 
{
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) 
	{
		// what to do when left key is pressed...

	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) 
	{

	}
	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) 
	{

	}
	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) 
	{

	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	* this function*/
	/*
	glutPostRedisplay();
	*/
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*
* You will have to add the necessary code here for finding the direction of shooting
*
* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*
* */

void MouseMoved(int x, int y) {
	//If mouse pressed then check than swap the cookies and if after swaping cookies dont brust then reswap the cookies

}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
*
* You will have to add the necessary code here for shooting, etc.
*
* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
* x & y that tells the coordinate of current position of move mouse
*
* */
 // Speed of the cookie movement
 // Function to check if the cookie collides with any alphabet in the given row

void MouseClicked(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{

		// Check for lanched state

		if(cookieLaunched) return;

		cookieSpeed = 30.0f;// Reset cookie speed
		// Convert the mouse position from window coordinates to OpenGL coordinates
		float xmouse = (float)x / width * 2 - 1;
		float ymouse = -((float)y / height * 2 - 1);

		// Calculate the direction from the cookie to the mouse position
		float dx = xmouse - (float)cookieX / width * 2 + 1;
		float dy = ymouse - (float)cookieY / height * 2 + 1;


		// Normalize the direction vector
		float length = sqrt(dx * dx + dy * dy);
		dx /= length;
		dy /= length;

		// Set the direction for the cookie movement
		cookieSpeedX = dx;
		cookieSpeedY = dy;

		cookieLaunched = true;  // cookie is launched
	}
}


void PrintableKeys(unsigned char key, int x, int y)
{
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)

* is pressed from the keyboard

* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the

* program coordinates of mouse pointer when key was pressed.

* */
}

/*
* This function is called after every 1000.0/FPS milliseconds
* (FPS is defined on in the beginning).
* You can use this function to animate objects and control the
* speed of different moving objects by varying the constant FPS.
*
* */
void Timer(int m) 
{
	if(!gameOver) 
	{
		elapsedTime++;

		if (elapsedTime >= gameDuration) gameOver = true; // Set the game over flag after 120 seconds

		glutPostRedisplay();

		glutTimerFunc(1000, Timer, 0);  
	}
}


///// smooth flow
void FrameRateTimer(int m)
{

	glutPostRedisplay();

	glutTimerFunc(1000 / FPS, FrameRateTimer, 0);  
}



/* our gateway main function */
int main(int argc, char*argv[]) 
{
	InitRandomizer(); // seed the random number generator...

	//Dictionary for matching the words. It contains the 370099 words.
	dictionary = new string[dictionarysize]; 
	ReadWords("words_alpha.txt", dictionary); // dictionary is an array of strings
	//print first 5 words from the dictionary
	for(int i=0; i < 5; ++i)
		cout<< " word "<< i << " =" << dictionary[i] <<endl;

	//Write your code here for filling the canvas with different Alphabets. You can use the Getalphabet function for getting the random alphabets


	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("Muanna Hamid"); // set the title of our game window
	//SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	RegisterTextures();
	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	GenerateAlphabetGrid();  // Generate fixed alphabet grid
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000, Timer, 0);
	glutTimerFunc(1000 / FPS, FrameRateTimer, 0);  // Add a timer for rendering
	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	glutMainLoop();
	return 1;
}
#endif /* */
