#include <stdlib.h>
#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include <mmsystem.h>
#include <windows.h>

#define M_PI 3.14159265358979323846

//Holds the x,y,z coordinates for any point used
struct Position {
	float x;
	float y;
	float z;
};

//Contains Planet attributes that make each unique planet
struct Planets {
	char name[20];
	struct Position p;
	float angleRotation;
	float angleOrbit;
	float radius;
	float colour[3];
	float velocityOrbit;
} solar[9] = { 
{ "Mercury", { 2*(0.35), 0, 0 }, 0,0, 2*(0.03f), { 0.55, 0.47, 0.14 }, 1.607 },
{ "Venus", { 2*(0.5), 0, 0 }, 0,0, 2*(0.05f), { 1, 0.5, 0 }, 1.174 },
{ "Earth", { 2*(0.67), 0, 0 }, 0,0, 2*(0.05f), { 0, 1.0, 0.498039 }, 1.00 },
{ "Mars", { 2*(0.85), 0, 0 }, 0, 0, 2*(0.05f), { 0.556863, 0.137255, 0.137255 }, 0.802 },
{ "Jupiter", { 2*(1.12), 0, 0 }, 0, 0, 2*(0.15f), { 0.435294, 0.258824, 0.258824 }, 0.434 },
{ "Saturn", { 2*(1.45), 0, 0 }, 0, 0, 2*(0.10f), { 0.6,  0.8, 0.196078 }, 0.323 },
{ "Uranus", { 2*(1.70), 0, 0 }, 0, 0, 2*(0.07f), { 0.184314, 0.184314, 0.309804 }, 0.228 },
{ "Neptune", { 2*(1.95), 0, 0 }, 0, 0, 2*(0.07f), { 0.137255, 0.137255, 0.556863 }, 0.182 },
{ "Pluto", { 2*(2.15), 0, 0 }, 0, 0, 2*(0.02f), { 0.752941, 0.752941, 0.752941 }, 0.159 } };

//Global roation speeds and angles for the moon
float speedMoon = 0.1;
float angleMoon = 0.0;

//Global variables to determine in the user pressed the button to turn on specific actions
int ringsOn = 0;
int starsOn = 0;
int sunShine = 0;
int musicOn = 0;
int sheildOn = 0;
int speedUp = 0;
int speedDown = 0;
int liveLongAnd = 0;
int alarm = 0;
int beamUp = 0;

//Global variables for moving the scene when pressed
int left = 0;
int right = 0;
int up = 0;
int down = 0;
int forward = 0;
int backward = 0;

//Global variables for moving the scene, are constantly added or subtracted to in order to translate the scene
float moveX = 0.0;
float moveY = 0.0;
float moveZ = 0.0;
float speedFactor = 1.0;
float rotateKling = 0.0;
float spinSun = 0.0;

void CreatePlanet() {
	glMatrixMode(GL_MODELVIEW);
	//draws each of the planets with their specific radius and position
	for (int i = 0; i < 9; i++){
		GLUquadric *quad;
		quad = gluNewQuadric();
		gluQuadricNormals(quad, GLU_SMOOTH);
		gluQuadricTexture(quad, GL_TRUE);

		//sets the colour to the specific planet
		glColor3f(solar[i].colour[0], solar[i].colour[1], solar[i].colour[2]);

		glPushMatrix();
		//rotation in order to orbit the planet around the sun
		glRotatef(solar[i].angleOrbit, 0.0, 1.0, 0.0);
		//translate the planets from the origin to their proper place in the solar system
		glTranslatef(solar[i].p.x, solar[i].p.y, solar[i].p.z);

		//drawing neptunes ring around the planet
		if (i == 6){
			glColor3f(1, 1, 1);
			glBegin(GL_LINE_STRIP);
			for (int j = 0; j < 361; j++) {
				glVertex3f(0, 0.2 * (float)cos(j * M_PI / 180), 0.2 * (float)sin(j * M_PI / 180));
			}
			glEnd();
			glColor3f(solar[i].colour[0], solar[i].colour[1], solar[i].colour[2]);
		}

		//rotates planets on their axis
		glRotatef(solar[i].angleRotation, 0, 1, 0);
		gluSphere(quad, solar[i].radius, 32, 32);

		//if the planet is earth, add the moon to it 
		if (i == 2){
			//draws the orbit ring for the moon around the earth
			if (ringsOn == 1){
				glColor3f(1, 1, 1);
				glBegin(GL_LINE_STRIP);
				for (int j = 0; j < 361; j++) {
					glVertex3f(0.2 * (float)sin(j * M_PI / 180), 0, 0.2 * (float)cos(j * M_PI / 180));
				}
				glEnd();
			}
			//draws the moon
			glColor3f(1, 1, 1);
			glRotatef(angleMoon, 0.0, 1.0, 0.0);
			glTranslatef(2*(0.1), 0, 0);
			glRotatef(solar[i].angleRotation, 0, 1, 0);
			gluSphere(quad, 0.019, 32, 32);

		}
		//drawing saturns ring around the planet
		if (i == 5){
			glColor3f(0.6, 0.8, 0.196078);
			glBegin(GL_QUAD_STRIP);
			for (int j = 0; j < 361; j++) {
				glVertex3f(2*(0.18) * (float)sin(j * M_PI / 180), 0, 2*(0.18) * (float)cos(j * M_PI / 180));
				glVertex3f(2*(0.12) * (float)sin(j * M_PI / 180), 0, 2*(0.12) * (float)cos(j * M_PI / 180));
			}
			glEnd();
		}
		glPopMatrix();
	}
}

//method responsible for drawing the orbial rings for each planets orbit around the sun
void DrawOrbit() {
	glColor3f(1, 1, 1);
	for (int i = 0; i < 9; i++) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < 361; j++) {
			glVertex3f(solar[i].p.x * (float)sin(j * M_PI / 180), 0, solar[i].p.x * (float)cos(j * M_PI / 180));
		}
		glEnd();
	}
}

//method responsible for drawing the suns "shimmering" effect
void DrawCorona() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int r;
	for (int i = 0; i < 1000; i++){
		r = (rand() % 360);
		glBegin(GL_LINES);
		glColor4f(1, 1, 0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glColor4f(1, 1, 0, 0.0);
		//generates vertex at random points along the sphere in order to create lines coming out
		glVertex3f(cos(r)*0.70, sin(r)*0.70, 0);
		glEnd();
	}
}

//responsible for the moving effect for orbits, spinning and translations
void myIdle() {
	angleMoon += speedMoon;
	//moves between 0-360 degrees
	if (angleMoon > 360){
		angleMoon -= 360;
	}
	for (int j = 0; j<9; j++){
		solar[j].angleRotation += 5.0;
		if (solar[j].angleRotation >360.0){
			solar[j].angleRotation -= 360.0;
		}
	}
	//speeds up orbit velocity
	if (speedUp == 1){
		speedFactor += 0.1;
	}
	//speeds down orbit velocity
	if (speedDown == 1){
		speedFactor -= 0.1;
		if (speedFactor < 0){
			speedFactor = 0;
		}
	}
	//controls angle/speed of planet orbit
	for (int i = 0; i < 9; i++){
		solar[i].angleOrbit += speedFactor*solar[i].velocityOrbit;
		if (solar[i].angleOrbit > 360){
			solar[i].angleOrbit -= 360;
		}
	}
	//controls angle/speed of klingon orbit
	rotateKling += speedFactor*0.1;
	if (rotateKling > 360){
			rotateKling -= 360;
	}

	//spins the sun on its axis
	spinSun += 5.0;
	if (spinSun > 360){
		spinSun -= 360;
	}

	//if left direction pressed, translate left
	if (left == 1){
		moveX += 0.02;
	}
	//if right direction pressed, translate right
	if (right == 1){
		moveX -= 0.02;
	}
	//if up direction pressed, translate up
	if (up == 1){
		moveY -= 0.02;
	}
	//if down direction pressed, translate down
	if (down == 1){
		moveY += 0.02;
	}
	//if pageup press, translate forward
	if (forward == 1){
		moveZ += 0.02;
	}
	//if pagedown pressed, translate backward
	if (backward == 1){
		moveZ -= 0.02;
	}
	glutPostRedisplay();

}

//create maximum 3500 stars
struct Position stars[3500];

//Method to produce randomly placed stars that shimmer in space
void CreateStars() {
	float r;
	int amount = 3500;
	int count = 0;

	//creates a point in a random (x,y,z) position
	while (count != amount) {
		r = -35.0 + ((float)rand() / (RAND_MAX / (35.0 - (-35.0))));
		stars[count].x = (r);
		r = -35.0 + ((float)rand() / (RAND_MAX / (35.0 - (-35.0))));
		stars[count].z = (r);
		r = -35.0 + ((float)rand() / (RAND_MAX / (35.0 - (-35.0))));
		stars[count].y = (r);
		count++;
	}
}

//Draws the created stars, swicthing colors to give shimmering effect
void DrawStars() {
	int r = rand() % 3;
	if (r == 1){
		glColor3f(1, 1, 1);
	}
	else if (r == 2){
		glColor3f(0, 1, 1);
	}
	else if (r == 3){
		glColor3f(0, 0, 1);
	}
	for (int i = 0; i < sizeof(stars)/sizeof(stars[0]); i++){
		glBegin(GL_POINTS);
		glPointSize(0.3);
		glVertex3f(stars[i].x, stars[i].y, stars[i].z);
		glEnd();
	}
}

/**********************************************Music Functions*****************************************************/

/*All sound files were obtained from http://www.mediacollege.com/downloads/sound-effects/star-trek/tos/ they do
not belong to me. All rights belong to Star Trek: The Original Series!*/

//Plays the star trek theme when toggled on
void musicButton(){
	sndPlaySound("Opening_Remastered.wav", SND_ASYNC);
}
//Plays alarm sound when toggled
void redAlert(){
	sndPlaySound("tos-redalert.wav", SND_ASYNC);
}
//plays transporter sound when toggled
void transport(){
	sndPlaySound("tos-transporter.wav", SND_ASYNC);
}
//plays live long and prosper when toggled
void liveLong(){
	sndPlaySound("Spock_Livelong.wav", SND_ASYNC);
}
/******************************************************************************************************************/

//2D array to hold all 1201 records of vertcies of x,y,z in enterprise.txt file
GLfloat vertex[1201][3];
//2D array to hold all 1989 number of faces within the enterprise.txt file, each face represents a index in the vertex array
int faces[1989][3];

//keeps track of how many vertexs were found
int countV = 0;
//keep track of how mnay faces were found
int countF = 0;

//loads the enterprise file on program startup
void loadEnterprise(){
	
	FILE * file = fopen("enterprise.txt", "r");
	if (file == NULL){
		printf("Impossible to open the file !\n");
		exit(0);
	}
	while (1){
		char firstword[128];
		int first = fscanf(file, "%s", firstword);
		if (first == EOF){
			break;
		}
		if (strcmp(firstword, "v") == 0){
			fscanf(file, "%f %f %f\n", &vertex[countV][0], &vertex[countV][1], &vertex[countV][2]);
			countV++;
		}
		if (strcmp(firstword, "f") == 0){
			fscanf(file, "%d %d %d\n", &faces[countF][0], &faces[countF][1], &faces[countF][2]);
			countF++;
		}
	}
}


//2D array to hold all 1670 records of vertcies of x,y,z in klingon.txt file
GLfloat vertexKling[1610][3];
//2D array to hold all 3191 number of faces within the klingon.txt file, each face represents a index in the vertex array
int facesKling[3191][3];
//keeps track of how many vertexs were found
int countVK = 0;
//keep track of how mnay faces were found
int countFK = 0;

//loadKlingon Ship from file
void loadKlingon(){

	FILE * file2 = fopen("klingon.txt", "r");
	if (file2 == NULL){
		printf("Impossible to open the file !\n");
		exit(0);
	}
	while (1){
		char firstword[128];
		int first = fscanf(file2, "%s", firstword);
		if (first == EOF){
			break;
		}
		if (strcmp(firstword, "v") == 0){
			fscanf(file2, "%f %f %f\n", &vertexKling[countVK][0], &vertexKling[countVK][1], &vertexKling[countVK][2]);
			countVK++;
		}
		if (strcmp(firstword, "f") == 0){
			fscanf(file2, "%d %d %d\n", &facesKling[countFK][0], &facesKling[countFK][1], &facesKling[countFK][2]);
			countFK++;
		}
	}

}

//draws the enterprise when called
void drawEnterprise(){
	glPushMatrix();
	//move the enterprise backwards from the solar system
	glTranslatef(0, 1.0, 6.0);
	glScalef(0.3, 0.3, 0.3);
	glRotatef(0, 0, 0, 0);
	//foreach face index in the faces array you find the specific vertex index that they call
	for (int i = 0; i < 1989; i++){
		glColor3f(i/1989.0, i/1989.0, i/1989.0);
		glBegin(GL_POLYGON);
		glVertex3fv(vertex[faces[i][0]-1]);
		glVertex3fv(vertex[faces[i][1]-1]);
		glVertex3fv(vertex[faces[i][2]-1]);
		glEnd();
	}
	glPopMatrix();
	
}

//draws the Klingon ship when called
void drawKlingon(){
	glPushMatrix();
	//move the enterprise backwards from the solar system
	//gives the klingon ship an orbit around the solar system, ITS PATROLLING!
	glRotatef(rotateKling, 0.0, 1.0, 0.0);
	glTranslatef(-10.0, 0.0, -15.0);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(270, 1.0, 0.0, 1.0);
	//foreach face index in the faces array you find the specific vertex index that they call
	for (int i = 0; i < 3191; i++){
		glColor3f(i / 3191.0, i / 3191.0, i / 3191.0);
		glBegin(GL_TRIANGLES);
		glVertex3fv(vertexKling[facesKling[i][0] - 1]);
		glVertex3fv(vertexKling[facesKling[i][1] - 1]);
		glVertex3fv(vertexKling[facesKling[i][2] - 1]);
		glEnd();
	}
	glPopMatrix();

}

//draws the sheild for the enterprise
void drawSheild(){
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glEnable(GL_BLEND);
	GLUquadric *quad;
	quad = gluNewQuadric();
	//position shield around enterprise
	glTranslatef(0, 1.0, 6.0);
	//chooses a r,b,g colour value for the sheild, makes it shimmer
	glColor4ub(rand() % 255, rand() % 255, rand() % 255, 32);
	glScalef(0.8, 0.8, 1.3);
	gluSphere(quad, 0.25, 32, 32);
	glPopMatrix();
	glDisable(GL_BLEND);
}

//draws the sun when called, lies on origin
void DrawSun(){
	glColor3f(1, 1, 0);
	GLUquadric *quad;
	quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);
	glPushMatrix();
	glRotatef(spinSun, 0, 1, 0);
	gluSphere(quad, 2*(0.25), 32, 32);
	glPopMatrix();
}

void display(void) {
	/* display callback, clear frame buffer and z buffer,
	rotate cube and draw, swap buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawEnterprise();
	glPushMatrix();
	//performs translations on the whole scene, but the enterprise to make it look as the ship is moving
	glTranslatef(moveX, moveY, moveZ);

	if (starsOn == 1){
		DrawStars();
	}
	if (ringsOn == 1){
		DrawOrbit();
	}
	//Drawing the sun
	DrawSun();
	glMatrixMode(GL_MODELVIEW);
	//drawing planets
	glDisable(GL_BLEND);
	CreatePlanet();
	if (sunShine == 1){
		DrawCorona();
		glDisable(GL_BLEND);
	}
	drawKlingon();
	glPopMatrix();
	//add sheild to enterprise if toggled
	if (sheildOn == 1){
		drawSheild();
	}
	glFlush();
	glutSwapBuffers();
}

//handles reshaping of the window
void myReshape(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (width <= height) {
		glOrtho(-2.0, 2.0, -2.0 * (GLfloat)height / (GLfloat)width, 2.0 * (GLfloat)height / (GLfloat)width, -10.0, 10.0);
		gluPerspective(50.0, 0.5*((GLfloat)width / (GLfloat)height), -1.0, 10.0);
	}
	else{
		glOrtho(-2.0 * (GLfloat)width / (GLfloat)height, 2.0 * (GLfloat)width / (GLfloat)height, -2.0, 2.0, -10.0, 10.0);
		gluPerspective(50.0, 0.5*((GLfloat)width / (GLfloat)height), -1.0, 10.0);
	}
	glMatrixMode(GL_MODELVIEW);
}

//inital setup values
void init(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0);
	loadEnterprise();
	loadKlingon();
}

//function to detect if the user pressed r,s,c,m,k,a,+,-,t and l
void keyPressed(unsigned char key, int x, int y) {
	if(key == 'r'){
		ringsOn = 1-ringsOn;
	}
	else if (key == 's'){
		starsOn = 1-starsOn;
	}
	else if (key == 'c'){
		sunShine = 1-sunShine;
	}
	//toggles music on, once music is going will not stop until finished
	else if (key == 'm'){
		musicOn = 1;
		if (musicOn == 1){
			musicButton();
		}
	}
	else if (key == 'k'){
		sheildOn = 1 - sheildOn;
	}
	else if (key == '+'){
		speedUp = 1;
	}
	else if (key == '-'){
		speedDown = 1;
	}
	else if (key == 'a'){
		alarm = 1;
		if (alarm == 1){
			redAlert();
		}
	}
	else if (key == 'l'){
		liveLongAnd = 1;
		if (liveLongAnd == 1){
			liveLong();
		}
	}
	else if (key == 't'){
		beamUp = 1;
		if (beamUp == 1){
			transport();
		}
	}
}

//determines in the user has unpressed a key
void depressedKey(unsigned char key) {
	if (key == '+'){
		speedUp = 0;
	}
	else if (key == '-'){
		speedDown = 0;
	}
	else if (key == 'm'){
		musicOn == 0;
	}
	else if (key == 'a'){
		alarm = 0;
	}
	else if (key == 't'){
		beamUp = 0;
	}
	else if (key == 'l'){
		liveLongAnd = 0;
	}
}

//function to determine if the user pressed up,down,left,right, pageup or pagedown
void processSpecialKeys(int key, int xx, int yy) {

	if (key == GLUT_KEY_LEFT){
		left = 1;
	}
	else if (key == GLUT_KEY_RIGHT){
		right = 1;
	}
	else if (key == GLUT_KEY_UP){
		up = 1;
	}
	else if (key == GLUT_KEY_DOWN){
		down = 1;
	}
	else if (key == GLUT_KEY_PAGE_UP){
		forward = 1;
	}
	else if (key == GLUT_KEY_PAGE_DOWN){
		backward = 1;
	}
}

//function to reset the values or up,down,left,right, forward and backwards to zero to stop the scene translation
void depressedSpecialKey(int key){
	if (key == GLUT_KEY_LEFT){
		left = 0;
	}
	else if (key == GLUT_KEY_RIGHT){
		right = 0;
	}
	else if (key == GLUT_KEY_UP){
		up = 0;
	}
	else if (key == GLUT_KEY_DOWN){
		down = 0;
	}
	else if (key == GLUT_KEY_PAGE_UP){
		forward = 0;
	}
	else if (key == GLUT_KEY_PAGE_DOWN){
		backward = 0;
	}
}

void main(int argc, char **argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 700);
	glutCreateWindow("Star Trek: The GL Generation");
	init();
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(processSpecialKeys);
	glutSpecialUpFunc(depressedSpecialKey);
	glutKeyboardUpFunc(depressedKey);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(myIdle);
	CreateStars();
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	//Prints out instructions on how to use the program
	printf("\nScene Controls\n-------------------------\nr: Turn orbit rings on\ns: Turn stars on\nc: Turn sun corona on\n");
	printf("k: Turn sheild on\nm: Turn music on!\n+: Hold to increase orbit speed\n-: Hold to decrease orbit speed\n");
	printf("\nCamera Controls\n-------------------------\nUp Arrow: Move up\nDown Arrow: Move down\nRight Arrow: Move right\n");
	printf("Left Arrow: Move left\nPage Up: Move forward\nPage Down: Move backwards\n");
	printf("\nEnterprise Functions\n-------------------------\na: RED ALERT!\nt: Beam me up, Scotty!\nl: Live long and Prosper");

	glutMainLoop();
}