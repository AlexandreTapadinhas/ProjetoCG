/*
Rui Alexandre Tapadinhas
2018283200
rui@student.dei.uc.pt
*/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>
#include "RgbImage.h"


//--------------------------------- Definir cores
#define BLUE     0.0, 0.0, 1.0, 1.0
#define RED		 1.0, 0.0, 0.0, 1.0
#define YELLOW	 1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define BROWN	 0.7, 0.3, 0.1, 1.0
#define GREY	 0.5, 0.5, 0.5, 1.0
#define LIGHTBLUE 0.2, 0.7, 1.0, 1.0
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

static GLfloat texturas[] = {
		1, 0,
		1, 1,
		0, 1,
		0, 0,
		0, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 1,
		0, 0,
		1, 0,
		1, 1,
		0, 0,
		0, 1,
		1, 0,
		1, 1,
		1, 1,
		1, 0,
		0, 1,
		0, 0 };

//------------------------------------------------------------ Cores
// static GLfloat cor[] = { };
// nao está feito !!


//=========================================================== FACES DO CUBO
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
char		texto[30];

//------------------------------------------------------------ Visualizacao/Observador 
GLfloat  rVisao = 10, aVisao = 0.5 * PI, incVisao = 0.05;
GLfloat  obsP[] = { rVisao * cos(aVisao), 3.0, rVisao * sin(aVisao) };
GLfloat  obsTarget[] = { 2.5, 5, 0.25 };
GLfloat  angZoom = 90;
GLfloat  incZoom = 3;


//------------------------------------------------------------ TEXTURAS
GLuint   texture[5];
RgbImage tex_img;

//------------------------------------------------------------ ILUMINAÇÃO
//Ambiente
GLint   Dia = 1;     //:::   'D'  
GLfloat intensidadeDia = 0.3;
GLfloat luzGlobalCorAmb[4] = { intensidadeDia, intensidadeDia,intensidadeDia, 1.0 };

//Pontual
GLint   ligaTeto = 1;		 //:::   'T'  
GLfloat intensidadeT = 0.3;  //:::   'I'  
GLint   luzR = 1;		 	 //:::   'R'  
GLint   luzG = 1;			 //:::   'G'  
GLint   luzB = 1;			 //:::   'B' 

GLint localPosx = 2.5;
GLint localPosy = 10;
GLint localPosz = 5;
GLfloat localPos[4] = { localPosx, localPosy, localPosz, 1.0 };
GLfloat localCorAmb[4] = { 0, 0, 0, 0.0 };
GLfloat localCorDif[4] = { luzR, luzG, luzB, 1.0 };
GLfloat localCorEsp[4] = { luzR, luzG, luzB, 1.0 };

//Focos
bool 		Focos[] = { 1,1 };		//.. Dois Focos ligados ou desligados
GLfloat		aberturaFoco = 10.0;		//.. angulo inicial do foco
GLfloat		anguloINC = 3.0;		//.. incremento
GLfloat		anguloMIN = 3.0;		//.. minimo
GLfloat		anguloMAX = 70.0;		//.. maximo

GLfloat Pos1[] = { 0.0f, 5.0f,  1.0f, 1.0f };   // Foco 1
GLfloat Pos2[] = { 5.0f, 5.0f,  1.0f, 1.0f };   // Foco 2 

// MALHA
GLint dim = 64;

void initTexturas()
{
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	tex_img.LoadBmpFile("madeira.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		tex_img.GetNumCols(),
		tex_img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		tex_img.ImageData());

	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	tex_img.LoadBmpFile("metal.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		tex_img.GetNumCols(),
		tex_img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		tex_img.ImageData());

	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	tex_img.LoadBmpFile("glass.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		tex_img.GetNumCols(),
		tex_img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		tex_img.ImageData());

	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	tex_img.LoadBmpFile("madeira_chao.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		tex_img.GetNumCols(),
		tex_img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		tex_img.ImageData());
}

void initLuzes(void) {
	//Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);

	//Pontual
	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);

	//Focos
	GLfloat Foco_direccao[] = { 0, -1, 0, 0 };	//……… -Y
	GLfloat Foco1_cor[] = { 0, 1,  0, 1 };	//……… Cor da luz 1
	GLfloat Foco2_cor[] = { 1, 0,  0, 1 };	//……… Cor da luz 2
	GLfloat Foco_ak = 1.0;
	GLfloat Foco_al = 0.05f;
	GLfloat Foco_aq = 0.0f;
	GLfloat Foco_Expon = 2.0;		// Foco, SPOT_Exponent

	//……………………………………………………………………………………………………………………………Foco Esquerda
	glLightfv(GL_LIGHT1, GL_POSITION, Pos1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Foco1_cor);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, Foco_ak);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, Foco_al);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, Foco_aq);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, aberturaFoco);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, Foco_direccao);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, Foco_Expon);

	//……………………………………………………………………………………………………………………………Foco Direita
	glLightfv(GL_LIGHT2, GL_POSITION, Pos2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Foco2_cor);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, Foco_ak);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, Foco_al);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, Foco_aq);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, aberturaFoco);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, Foco_direccao);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, Foco_Expon);

}

void updateLuz() {
	localCorAmb[0] = luzR * intensidadeT;
	localCorAmb[1] = luzG * intensidadeT;
	localCorAmb[2] = luzB * intensidadeT;
	localCorDif[0] = luzR * intensidadeT;
	localCorDif[1] = luzG * intensidadeT;
	localCorDif[2] = luzB * intensidadeT;
	localCorEsp[0] = luzR * intensidadeT;
	localCorEsp[1] = luzG * intensidadeT;
	localCorEsp[2] = luzB * intensidadeT;;
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);
}

//================================================================================
//=========================================================================== INIT
void inicializa(void)
{
	glClearColor(LIGHTBLUE);		//………………………………………………………………………………Apagar
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);	//………………………………………………………………………………Profundidade
	glShadeModel(GL_SMOOTH);	//………………………………………………………………………………Interpolacao de cores

	initTexturas();
	initLuzes();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);


	glVertexPointer(3, GL_FLOAT, 0, vertices); //………………………………………VertexArrays: vertices + normais + cores
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normais);
	glEnableClientState(GL_NORMAL_ARRAY);
	// falta cores
	glTexCoordPointer(2, GL_FLOAT, 0, texturas);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

}

void desenhaTexto(char* string, GLfloat x, GLfloat y, GLfloat z) {
	glRasterPos3f(x, y, z);
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *string++);
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
	glTexCoordPointer(2, GL_FLOAT, 0, texturas);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);

	//…………………………………………………………………………………………………………………………………………… Teto
	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	// Pontual
	if (ligaTeto) {
		glPushMatrix();
		glColor4f(WHITE);
		glTranslatef(localPosx, localPosy, localPosz);
		glScalef(0.25, 0.25, 0.25);
		drawCube();
		glPopMatrix();
	}
	//Focos
	if (Focos[0]) { //Verde
		glPushMatrix();
		glColor3f(0.0f, 1.0f, 0.0f);
		glTranslatef(Pos1[0], Pos1[1], Pos1[2]);
		glScalef(0.25, 0.25, 0.25);
		drawCube();
		glPopMatrix();
	}
	if (Focos[1]) { 	//Vermelho
		glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glTranslatef(Pos2[0], Pos2[1], Pos2[2]);
		glScalef(0.25, 0.25, 0.25);
		drawCube();
		glPopMatrix();
	}
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	//==================================== Porta
	glPushMatrix();
	glRotatef(anguloPorta, 0, 1, 0); //rotação do conjunto todo

	glPushMatrix();
	//glColor4f(BROWN);
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
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();//peça 1
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTranslatef(1, 5, 0.25);
	glScalef(2, 10, 0.5);
	drawCube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();//peça 2
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTranslatef(4, 5, 0.25);
	glScalef(2, 10, 0.5);
	drawCube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();//peça 3
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTranslatef(2.5, 9, 0.25);
	glScalef(1, 2, 0.5);
	drawCube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();//peça 4
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTranslatef(2.5, 3.5, 0.25);
	glScalef(1, 7, 0.5);
	drawCube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();



	GLfloat corTransp[] = { 1.0, 1.0, 1.0, 0.5 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, corTransp);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, corTransp);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, corTransp);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Janela da porta
	glPushMatrix();
	glTranslatef(xDeslizaJanela, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	//glColor4f(BLACK);
	glTranslatef(2.5, 7.5, 0.0375);
	glScalef(1, 1, 0.05);
	drawCube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glBlendFunc(GL_ONE, GL_ZERO);




	//Maçaneta
	GLfloat macanetaAmb[] = { 0.23125f, 0.23125f, 0.23125f, 1.0f };
	GLfloat	macanetaDif[] = { 0.50754f, 0.50754f, 0.50754f, 1.0f };
	GLfloat	macanetaSpec[] = { 0.508273f, 0.508273f, 0.508273f, 1.0f };
	GLfloat macanetaCoef = 51.2f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, macanetaAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, macanetaDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, macanetaSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, macanetaCoef);

	glPushMatrix();
	glTranslatef(4.2, 5, 0.8);
	glTranslatef(0.60 / 2, 0, 0);
	glRotatef(anguloMacaneta, 0, 0, 1);
	glTranslatef(-0.60 / 2, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();//Paralelipipedo
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	//glColor4f(GREY);
	glTranslatef(0, 0, 0);
	glScalef(0.75, 0.2, 0.2);
	drawCube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();//Cubo
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	//glColor4f(GREY);
	glTranslatef(0.275, 0, -0.2);
	glScalef(0.2, 0.2, 0.2);
	drawCube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPopMatrix(); //fim rotação porta

	//Chão
	GLfloat chaoAmb[] = { 0.2f,0.15f,0.15f,1.0f };
	GLfloat	chaoDif[] = { 0.3f,0.2f,0.2f,1.0f };
	GLfloat	chaoSpec[] = { 0.7f,0.6f,0.6f,1.0f };
	GLfloat chaoCoef = 32.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, chaoAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, chaoDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, chaoSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, chaoCoef);


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	float med_dim = (float)dim / 2;

	glPushMatrix();

	glNormal3f(0, 1, 0);          //normal 
	glTranslatef(-12.5, 0, -15);
	glScaled(15, 0, 15);

	glBegin(GL_QUADS);
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++) {
			glTexCoord2f((float)j / dim, (float)i / dim);				glVertex3d((float)j / med_dim, 0, (float)i / med_dim);
			glTexCoord2f((float)(j + 1) / dim, (float)i / dim);			glVertex3d((float)(j + 1) / med_dim, 0, (float)i / med_dim);
			glTexCoord2f((float)(j + 1) / dim, (float)(i + 1) / dim);	glVertex3d((float)(j + 1) / med_dim, 0, (float)(i + 1) / med_dim);
			glTexCoord2f((float)j / dim, (float)(i + 1) / dim);			glVertex3d((float)j / med_dim, 0, (float)(i + 1) / med_dim);
		}
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	/*
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTranslatef(2.5, 0, 0);
	glScalef(15, 0, 15);
	drawCube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	*/

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void iluminacao() {
	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);
	if (ligaTeto)  glEnable(GL_LIGHT0);
	else   		  glDisable(GL_LIGHT0);
}

void display(void) {

	//================================================================= Apaga ecran e lida com profundidade (3D)
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	//=============================================== Viewport0 Informacao - sem corIluminacap
	glDisable(GL_LIGHTING);
	glViewport(0, 400, wScreen / 6, hScreen / 5);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-xC, xC, -xC, xC, -zC, zC);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 5, 0, 0, 0, 0, 0, 0, -1);

	//------------------------------------------------ Informacao
	glColor3f(1, 1, 1);			// nao se está a usar LUZ !!   DisableLight
	sprintf_s(texto, 30, "%d - Dia 'D' ", Dia);
	desenhaTexto(texto, -12, 1, -14);
	sprintf_s(texto, 30, "%d - Teto 'T' ", ligaTeto);
	desenhaTexto(texto, -12, 1, -8);
	sprintf_s(texto, 30, "   %d%d%d - Luz 'RGB' ", luzR, luzG, luzB);
	desenhaTexto(texto, -12, 1, -5);
	sprintf_s(texto, 30, "   %4.2f - Intensidade 'I'", intensidadeT);
	desenhaTexto(texto, -12, 1, -2);
	glEnable(GL_LIGHTING);
	*/
	//================================================================= Viewport1 
	glViewport(0, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angZoom, (float)wScreen / hScreen, 0.1, (float)(zC * 3));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], obsTarget[0], obsTarget[1], obsTarget[2], 0, 1, 0);
	//================================================================= 


	//…………………………………………………………………………………………………………………………………………………………Objectos
	iluminacao();
	initLuzes();
	//drawEixos();
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
		obsTarget[0] = 5 / 2 * cos(DegToRad(anguloPorta));      // actualiza posicao (X,Z)
		obsTarget[2] = 5 / 2 * sin(DegToRad(anguloPorta));
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
		//LUZES
		//--------------------------- AMBIENTE
	case '1':
		Dia = !Dia;
		//printf("%d", Dia);
		if (Dia) { luzGlobalCorAmb[0] = 0.7;   luzGlobalCorAmb[1] = 0.7;   luzGlobalCorAmb[2] = 0.7; }
		else { luzGlobalCorAmb[0] = 0.2;   luzGlobalCorAmb[1] = 0.2;   luzGlobalCorAmb[2] = 0.2; }
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);
		glutPostRedisplay();
		break;
		//--------------------------- TETO
	case '2':
		ligaTeto = !ligaTeto;
		updateLuz();
		glutPostRedisplay();
		break;
		//--------------------------- Iluminacaoda sala
	case '3':
		intensidadeT = intensidadeT + 0.1;
		if (intensidadeT > 1.1) intensidadeT = 0;
		updateLuz();
		glutPostRedisplay();
		break;
	case '4':
		luzR = (luzR + 1) % 2;

		updateLuz();
		glutPostRedisplay();
		break;
	case '5':
		luzG = (luzG + 1) % 2;
		updateLuz();
		glutPostRedisplay();
		break;
	case '6':
		luzB = (luzB + 1) % 2;
		updateLuz();
		glutPostRedisplay();
		break;
	case '7': //liga desliga foco 1
		Focos[0] = !Focos[0];
		if (Focos[0] == 0)
			glDisable(GL_LIGHT1);
		else
			glEnable(GL_LIGHT1);
		glutPostRedisplay();
		break;
	case '8': //liga desliga foco 2
		Focos[1] = !Focos[1];
		if (Focos[1] == 0)
			glDisable(GL_LIGHT2);
		else
			glEnable(GL_LIGHT2);
		glutPostRedisplay();
		break;
	case '9':
		//printf("%f", aberturaFoco);
		aberturaFoco = aberturaFoco + anguloINC;
		if (aberturaFoco > anguloMAX)
			aberturaFoco = anguloMAX;
		glutPostRedisplay();
		break;
	case '0':
		//printf("%f", aberturaFoco);
		aberturaFoco = aberturaFoco - anguloINC;
		if (aberturaFoco < anguloMIN)
			aberturaFoco = anguloMIN;
		glutPostRedisplay();
		break;

	case 'm':
	case 'M':
		dim = 0.5 * dim;
		if (dim < 1) dim = 1;
		glutPostRedisplay();
		break;
	case 'n':
	case 'N':
		dim = 2 * dim;
		if (dim > 256) dim = 256;
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
