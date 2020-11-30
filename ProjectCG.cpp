/*
Rui Alexandre Tapadinhas
2018283200
rui@student.dei.uc.pt
*/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>


//--------------------------------- Definir cores
#define BLUE     0.0, 0.0, 1.0, 1.0
#define RED		 1.0, 0.0, 0.0, 1.0
#define YELLOW	 1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define BROWN	 0.7, 0.3, 0.1, 1.0
#define GREY	 0.5, 0.5, 0.5, 1.0
#define LIGHTBLUE 0.2, 0.7, 1.0 , 1.0
#define PI		 3.14159

//================================================================================
//===========================================================Variaveis e constantes

GLfloat tam = 0.5;
static GLfloat vertices[] = {
	//…………………………………………………… Esquerda
		-tam,  -tam,  tam,	// 0 
		-tam,   tam,  tam,	// 1 
		-tam,   tam, -tam,	// 2 
		-tam,  -tam, -tam,	// 3 
	//…………………………………………………… Direita
		 tam,  -tam,  tam,	// 4 
		 tam,   tam,  tam,	// 5 
		 tam,   tam, -tam,	// 6 
		 tam,  -tam, -tam,	// 7 
	//……………………………………………………… Cima
		-tam,  tam,  tam,	// 8 
		-tam,  tam, -tam,	// 9 
		 tam,  tam, -tam,	// 10 
		 tam,  tam,  tam,	// 11 
	//……………………………………………………… Baixo
		-tam,  -tam,  tam,	// 12
		-tam,  -tam, -tam,	// 13
		 tam,  -tam, -tam,	// 14 
		 tam,  -tam,  tam,	// 15 
	//……………………………………………………… Frente
		-tam,  -tam,  tam,	// 16 
		-tam,   tam,  tam,	// 17
		 tam,  -tam,  tam,	// 18 
		 tam,   tam,  tam,	// 19 
	//……………………………………………………… Tras
		-tam,   tam, -tam,	// 20 
		-tam,  -tam, -tam,	// 21 
		 tam,   tam, -tam,	// 22 
		 tam,  -tam, -tam,	// 23 

};

GLfloat height = 2;
GLfloat width = 0.5;


static GLfloat normais[] = {
	     //…………………………………………………………………………………………………… x=tam (Esquerda)
		-1.0,  0.0,  0.0,
		-1.0,  0.0,  0.0,
		-1.0,  0.0,  0.0,
		-1.0,  0.0,  0.0,
	    //…………………………………………………………………………………………………… x=tam (Direita)
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		//…………………………………………………………………………………………………… y=tam (Cima)
		0.0,  1.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  1.0,  0.0,
	    //…………………………………………………………………………………………………… Baixo
		0.0,  -1.0,  0.0,
		0.0,  -1.0,  0.0,
		0.0,  -1.0,  0.0,
		0.0,  -1.0,  0.0,
		//…………………………………………………………………………………………………… Frente
		0.0,  0.0,  1.0,
		0.0,  0.0,  1.0,
		0.0,  0.0,  1.0,
		0.0,  0.0,  1.0,
		//…………………………………………………………………………………………………… Tras
		0.0,  0.0,  -1.0,
		0.0,  0.0,  -1.0,
		0.0,  0.0,  -1.0,
		0.0,  0.0,  -1.0,

};

//------------------------------------------------------------ Cores
// static GLfloat cor[] = { };
// nao está feito !!


//=========================================================== FACES DA MESA
GLboolean   frenteVisivel = 0;
static GLuint	  esquerda[] = { 0, 1, 2, 3 };	// regra da mao direita
static GLuint	  direita[] = { 4, 7, 6, 5 };   // versor da face virado para o lado de fora da face de acordo com a regra da mao direita
static GLuint     cima[] = { 8, 11, 10, 9 };
static GLuint	  baixo[] = { 14, 15, 12, 13 };
static GLuint	  frente[] = { 18, 19, 17, 16 };
static GLuint	  tras[] = { 21, 20, 22, 23 };

GLfloat anguloPorta = 0;
GLfloat anguloMacaneta = 0;
GLfloat xDeslizaJanela = 0;
//------------------------------------------------------------ Objectos (sistema coordenadas)
GLint		wScreen = 800, hScreen = 600;			//.. janela (pixeis)
GLfloat		xC = 10.0, yC = 20.0, zC = 10.0;		//.. Mundo  (unidades mundo)

//------------------------------------------------------------ Visualizacao/Observador 
GLfloat  rVisao = 10, aVisao = 0.5 * PI, incVisao = 0.05;
GLfloat  obsP[] = { rVisao * cos(aVisao), 3.0, rVisao * sin(aVisao) };
GLfloat  obsTarget[] = {2.5, 5, 0.25};
GLfloat  angZoom = 90;
GLfloat  incZoom = 3;



//================================================================================
//=========================================================================== INIT
void inicializa(void)
{
	glClearColor(LIGHTBLUE);		//………………………………………………………………………………Apagar
	glEnable(GL_DEPTH_TEST);	//………………………………………………………………………………Profundidade
	glShadeModel(GL_SMOOTH);	//………………………………………………………………………………Interpolacao de cores	

	glVertexPointer(3, GL_FLOAT, 0, vertices); //………………………………………VertexArrays: vertices + normais + cores
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normais);
	glEnableClientState(GL_NORMAL_ARRAY);
	// cores ??
}

void drawCube() {
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
}

void drawEixos()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo X
	glColor4f(RED);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(10, 0, 0);
	glEnd();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Y
	glColor4f(GREEN);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 10, 0);
	glEnd();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Z
	glColor4f(BLUE);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 10);
	glEnd();

}

float DegToRad(float angDeg) {
	return ((angDeg * PI) / 180);
}

void drawScene() {

	glVertexPointer(3, GL_FLOAT, 0, vertices); //………………………………………VertexArrays: vertices + normais + cores
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normais);
	glEnableClientState(GL_NORMAL_ARRAY);


   //==================================== Porta

	glRotatef(anguloPorta, 0, 1, 0); //rotação do conjunto todo
	glPushMatrix();
		glColor4f(BROWN);
		//porta de 1 peça
		/*glTranslatef(2.5, 5, 0.25);
		glScalef(5, 10, 0.5);
		drawCube();*/
		//porta de 4 peças
		/*
			_____________
			|	| 3 |	|
			|	|___|	|
			|	|	|	|
			|	|___|	|
			| 1 |	| 2 |
			|	|	|	|
			|	| 4 |	|
			|	|	|	|
			|___|___|___|
		
		*/
		glPushMatrix();//peça 1
			glTranslatef(1, 5, 0.25);
			glScalef(2, 10, 0.5);
			drawCube();
		glPopMatrix();

		glPushMatrix();//peça 2
			glTranslatef(4, 5, 0.25);
			glScalef(2, 10, 0.5);
			drawCube();
		glPopMatrix();

		glPushMatrix();//peça 3
			glTranslatef(2.5, 9, 0.25);
			glScalef(1, 2, 0.5);
			drawCube();
		glPopMatrix();

		glPushMatrix();//peça 4
			glTranslatef(2.5, 3.5, 0.25);
			glScalef(1, 7, 0.5);
			drawCube();
		glPopMatrix();

	glPopMatrix();

	//Porta da janela
	glPushMatrix();
		glTranslatef(xDeslizaJanela, 0, 0);
		glPushMatrix();
			glColor4f(BLACK);
			glTranslatef(2.5, 7.5, 0.0375);
			glScalef(1, 1, 0.05);
			drawCube();
		glPopMatrix();
	glPopMatrix();

	//Maçaneta
	glTranslatef(4.2, 5, 0.8);
	glTranslatef(0.60 / 2, 0, 0);
	glRotatef(anguloMacaneta, 0, 0, 1);
	glTranslatef(-0.60/2, 0, 0);
	glPushMatrix();//Paralelipipedo
		glColor4f(GREY);
		glTranslatef(0, 0, 0);
		glScalef(0.75, 0.2, 0.2);
		drawCube();
	glPopMatrix();
	glPushMatrix();//Cubo
		glColor4f(GREY);
		glTranslatef(0.275, 0, -0.2);
		glScalef(0.2, 0.2, 0.2);
		drawCube();
	glPopMatrix();
	
}

void display(void) {

	//================================================================= APaga ecran e lida com profundidade (3D)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//================================================================= 
	glViewport(0, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angZoom, (float)wScreen / hScreen, 0.1, 3 * zC);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], obsTarget[0], obsTarget[1], obsTarget[2], 0, 1, 0);
	//================================================================= 


	//…………………………………………………………………………………………………………………………………………………………Objectos
	drawEixos();
	drawScene();

	//. . . . . . . . . . . . . . . . . . . . .  Actualizacao
	glutSwapBuffers();
}

//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y) {

	switch (key) {
		case 'a':
		case 'A':
			anguloPorta -= 5;
			if (anguloPorta < -120) {
				anguloPorta = -120;
			}
			obsTarget[0] = 5/2 * cos(DegToRad(anguloPorta));      // actualiza posicao (X,Z)
			obsTarget[2] = 5/2 * sin(DegToRad(anguloPorta));
			glutPostRedisplay();
			break;

		case 'd':
		case 'D':
			anguloPorta += 5;
			if (anguloPorta > 0) {
				anguloPorta = 0;
			}
			obsTarget[0] = 5 / 2 * cos(DegToRad(anguloPorta));      // actualiza posicao (X,Z)
			obsTarget[2] = 5 / 2 * sin(DegToRad(anguloPorta));
			glutPostRedisplay();
			break;

		case 's':
		case 'S':
			anguloMacaneta += 5;
			if (anguloMacaneta > 45) {
				anguloMacaneta = 45;
			}
			glutPostRedisplay();
			break;

		case 'w':
		case 'W':
			anguloMacaneta -= 5;
			if (anguloMacaneta < 0) {
				anguloMacaneta = 0;
			}
			glutPostRedisplay();
			break;
		case 'e':
		case 'E':
			xDeslizaJanela += 0.05;
			if (xDeslizaJanela > 0) {
				xDeslizaJanela = 0;
			}
			glutPostRedisplay();
			break;

		case 'q':
		case 'Q':
			xDeslizaJanela -= 0.05;
			if (xDeslizaJanela <= -1) {
				xDeslizaJanela = -1;
			}
			glutPostRedisplay();
			break;

	
		case 'z':
		case 'Z':
			if (rVisao < 25) {
				rVisao += 0.5;
			}
			obsP[0] = rVisao * cos(aVisao);      // actualiza posicao (X,Z)
			obsP[2] = rVisao * sin(aVisao);
			glutPostRedisplay();
			break;

	
		case 'x':
		case 'X':
			if (rVisao > 5) {
				rVisao -= 0.5;
			}
			obsP[0] = rVisao * cos(aVisao);      // actualiza posicao (X,Z)
			obsP[2] = rVisao * sin(aVisao);
			glutPostRedisplay();
			break;
		//--------------------------- Escape
		case 27:
			exit(0);
			break;
	}
	
}


void teclasNotAscii(int key, int x, int y) {
	//.. observador pode andar à volta da cena  (setas esquerda / direita)
	//.. observador pode andar para cima e para baixo (setas cima / baixo )

	if (key == GLUT_KEY_UP)   obsP[1] = (obsP[1] + 0.1);
	if (key == GLUT_KEY_DOWN) obsP[1] = (obsP[1] - 0.1);
	if (key == GLUT_KEY_LEFT)  aVisao = (aVisao + 0.03);
	if (key == GLUT_KEY_RIGHT) aVisao = (aVisao - 0.03);

	if (obsP[1] > yC)   obsP[1] = yC;   // limita altura
	if (obsP[1] < -yC)  obsP[1] = -yC;

	obsP[0] = rVisao * cos(aVisao);      // actualiza posicao (X,Z)
	obsP[2] = rVisao * sin(aVisao);

	glutPostRedisplay();
}


//======================================================= MAIN
//======================================================= MAIN
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(300, 100);
	glutCreateWindow("||Rui Tapadinhas - 2018283200|| ||Porta:'A'/'D'|| ||Maçaneta:'W'/'S'|| ||Janela:'Q'/'E'|| ||Observador:'SETAS'|| ||Zoom: 'Z'/'X'||");

	inicializa();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}
