#include <AntTweakBar.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include <GL\glut.h>
#include <time.h>
#include <vector>

#include "BasicShape.h"
#include "modifier.h"

#include "glm\glm.hpp"

int /*w = 640*/ w = 860, h = 640;
vector<BasicShape> shapes;
vector<Modifier> marbles;
vector<glm::vec2> coord;
int numberOfLayers = 0;

TwType layerType;
string enum_values;

int selectedLayer;
int selectedMarblingType = 0;

TwBar *bar;
float rad = 1.0;
BasicShape tempShape;
Modifier tempMod;

float layerCenter[3];
float centerX, centerY, centerZ;

float *layerColor = new float[3];
float *col = new float[3];

float lightPosition[4], lightAmbient[4], lightDiffuse[4];

bool DF = false;
bool modify = 0;
string modifyButtonText;
string modifyButtonLabel; 
int buttonClick;

float dist;
glm::vec3 shapeColor;
glm::vec2 currentPoint;

float omega = 80, lambda = 32;
float d;
glm::vec2 lineDir = glm::normalize(glm::vec2(0, -1));
glm::vec2 n;
int localx, localy;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_POINTS);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			glColor3f(0.5, 0.5, 0.5);
			
			currentPoint = coord[i * w + j];
			for (int k = marbles.size() - 1; k >= 0; k--) {
				currentPoint = marbles[k].doMarble(currentPoint);
			}			
		//	coord[i * w + j] = currentPoint;

			for (int k = 0; k < shapes.size(); k++) {
				dist = glm::length(currentPoint - shapes[k].getCenter());
				if (dist <= shapes[k].getRadius()) {
					shapeColor = shapes[k].getColor();
					glColor3f(shapeColor[0], shapeColor[1], shapeColor[2]);
					break;
				}
				else {
					float coeff = pow((1 - (pow(shapes[k].getRadius(), 2.0) / pow(dist, 2.0))), 0.5);
					currentPoint = shapes[k].getCenter() +
						(currentPoint - shapes[k].getCenter()) *
						coeff;
				}
			}
			glVertex2f(j - w / 2, h / 2 - i);
		}
	}
	glEnd();

	
	TwDraw();

	glutSwapBuffers();
}

void OnMouseClick(int button, int state, int x, int y)
{
	if (!TwEventMouseButtonGLUT(button, state, x, y)) {

		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				localx = x - (w / 2);
				localy = (h / 2) - y;
				glm::vec3 color;
				for (int i = 0; i < 3; i++)
					color[i] = rand() % 255 / 255.0;
				tempShape.set("", 50, glm::vec2(localx, localy), numberOfLayers, color);
				numberOfLayers += 1;
				shapes.insert(shapes.begin(), tempShape);
				selectedLayer = 0;

				enum_values = "";
				for (int i = 0; i < numberOfLayers; i++) {
					enum_values = enum_values + to_string(i) + " {" + to_string(shapes[i].getLayID()) + "}";
					if (i != numberOfLayers - 1)
						enum_values = enum_values + ", ";
				}

				TwSetParam(bar, "LayerSelection", "enum", TW_PARAM_CSTRING, 1, enum_values.c_str());
			}
			else {
				DF = false;
			}
		}
		else if (button == GLUT_RIGHT_BUTTON) {
			if (state == GLUT_DOWN) {
				localx = x - (w / 2);
				localy = (h / 2) - y;
				//modify = true;
			}
			else {
				lineDir = glm::normalize(glm::vec2((x - w / 2) - localx, (h / 2 - y) - localy));
				//cout << "Line Direction: " << lineDir[0] << ", " << lineDir[1] << ", " << lineDir[2] << endl;
				if (selectedMarblingType == 0) {
					tempMod.setLine(glm::vec2(localx, localy), lineDir);
				}
				else if (selectedMarblingType == 1) {
					tempMod.setComb(glm::vec2(localx, localy), lineDir, 20);
				}
				else if (selectedMarblingType == 2) {
					tempMod.setWavy();
				}
				else if (selectedMarblingType == 3) {
					tempMod.setCircular(glm::vec2(localx, localy), 40);
				}
				else if (selectedMarblingType == 4) {
					tempMod.setVortex(glm::vec2(localx, localy));
				}
					
				marbles.push_back(tempMod);
			}
		}
	}
}
void OnMouseMove(int x, int y)
{
	/*if (!TwEventMouseMotionGLUT(x, y) && modify) {
		lineDir = glm::normalize(glm::vec3((x - w / 2) - localx, (h / 2 - y) - localy, 0));
		localx = x - (w / 2);
		localy = (h / 2) - y;
	}*/

	//glutPostRedisplay();
}

void OnKeyboard(unsigned char key, int x, int y)
{
	if (!TwEventKeyboardGLUT(key, x, y)) {
		if (key == 'p') {
			DF = true;
			cout << "DF = true" << endl;
		}
	}
}

void init() {
	glClearColor(0, 0, 0, 1);

	selectedLayer = -1;

	/*tempShape.set("", 5, glm::vec2(0, 0), numberOfLayers, glm::vec3(0.5, 0.5, 0.5));
	shapes.push_back(tempShape);*/

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			coord.push_back(glm::vec2(j - w / 2, h / 2 - i));
		}
	}

	modifyButtonText = "Start Marbling";
	modifyButtonLabel = "label = '" + modifyButtonText + "' ";
}

//AntTweakBar callback functions
void TW_CALL modifyButtonClick(void *clientdata) {
	cout << "Modify Button Clicked" << endl;
	if (modify) {
		modify = false;
		modifyButtonText = "Start Marbling";
	}
	else {
		modify = true;
		modifyButtonText = "Stop Marbling";
	}

	modifyButtonLabel = "ControlBar/modifyButton label = '" + modifyButtonText + "' ";
	TwDefine(modifyButtonLabel.c_str());

	glutPostRedisplay();

	//cout << *static_cast<const int *>(clientdata);
}

void TW_CALL setBasicShapeRadius(const void *value, void *clientData) {
	if (selectedLayer != -1) {
		shapes[selectedLayer].setRadius(*static_cast<const float *>(value));
	}
}

void TW_CALL getBasicShapeRadius(void *value, void *clientData) {
	if (selectedLayer != -1) {
		*static_cast<float *>(value) = shapes[selectedLayer].getRadius();
	}
}

void TW_CALL setBasicShapeCenterX(const void *value, void *clientData) {
	if (selectedLayer != -1) {
		shapes[selectedLayer].setCenterX(*static_cast<const float *>(value));
	}
}

void TW_CALL getBasicShapeCenterX(void *value, void *clientData) {
	if (selectedLayer != -1) {
		*static_cast<float *>(value) = shapes[selectedLayer].getCenterX();
	}
}

void TW_CALL setBasicShapeCenterY(const void *value, void *clientData) {
	if (selectedLayer != -1) {
		shapes[selectedLayer].setCenterY(*static_cast<const float *>(value));
	}
}

void TW_CALL getBasicShapeCenterY(void *value, void *clientData) {
	if (selectedLayer != -1) {
		*static_cast<float *>(value) = shapes[selectedLayer].getCenterY();
	}
}

//void TW_CALL setBasicShapeCenterZ(const void *value, void *clientData) {
//	if (selectedLayer != -1) {
//		shapes[selectedLayer].setCenterZ(*static_cast<const float *>(value));
//	}
//}

void TW_CALL getBasicShapeCenterZ(void *value, void *clientData) {
	if (selectedLayer != -1) {
		*static_cast<float *>(value) = shapes[selectedLayer].getCenterZ();
	}
}

void TW_CALL setBasicShapeColorR(const void *value, void *clientData) {
	if (selectedLayer != -1) {
		shapes[selectedLayer].setColorR(*static_cast<const float *>(value));
	}
}

void TW_CALL getBasicShapeColorR(void *value, void *clientData) {
	if (selectedLayer != -1) {
		*static_cast<float *>(value) = shapes[selectedLayer].getColorR();
	}
}

void TW_CALL setBasicShapeColorG(const void *value, void *clientData) {
	if (selectedLayer != -1) {
		shapes[selectedLayer].setColorG(*static_cast<const float *>(value));
	}
}

void TW_CALL getBasicShapeColorG(void *value, void *clientData) {
	if (selectedLayer != -1) {
		*static_cast<float *>(value) = shapes[selectedLayer].getColorG();
	}
}

void TW_CALL setBasicShapeColorB(const void *value, void *clientData) {
	if (selectedLayer != -1) {
		shapes[selectedLayer].setColorB(*static_cast<const float *>(value));
	}
}

void TW_CALL getBasicShapeColorB(void *value, void *clientData) {
	if (selectedLayer != -1) {
		*static_cast<float *>(value) = shapes[selectedLayer].getColorB();
	}
}


int main(int argc, char *argv[]) {
	srand(time(NULL));

	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(w, h);
	glutCreateWindow("Solid Mathematical Marbling");
	glutCreateMenu(NULL);

	glViewport(0, 0, w, h);
	
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, h / w, 0.1f, 16000.0f);*/

	gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);

	TwWindowSize(w, h);

	init();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMouseFunc(OnMouseClick);
	glutMotionFunc(OnMouseMove);
	glutKeyboardFunc(OnKeyboard);

	// Initialize AntTweakBar
	TwInit(TW_OPENGL, NULL);

	TwGLUTModifiersFunc(glutGetModifiers);

	bar = TwNewBar("ControlBar");
	TwDefine(" GLOBAL help = 'Solid Mathematical Marbling' "); // Message added to the help bar.
	TwDefine(" ControlBar size='200 400' color='96 216 224' position = '650 16' "); // change default tweak bar size and color

	//add combobox for the layers
	layerType = TwDefineEnum("LayerType", NULL, 0);
	TwAddVarRW(bar, "LayerSelection", layerType, &selectedLayer, enum_values.c_str());
	//TwAddVarCB(bar, "LayerSelection", layerType, NULL, getBasicShapeRadius, &selectedLayer, enum_values.c_str());

	//label and textBox for radius
	TwAddVarCB(bar, "Radius", TW_TYPE_FLOAT, setBasicShapeRadius, getBasicShapeRadius, &rad, "");

	//label and textbox for center
	TwAddVarCB(bar, "centerX", TW_TYPE_FLOAT, setBasicShapeCenterX, getBasicShapeCenterX, &centerX, "group = 'Center' step = 0.01 ");
	TwAddVarCB(bar, "centerY", TW_TYPE_FLOAT, setBasicShapeCenterY, getBasicShapeCenterY, &centerY, "group = 'Center' step = 0.01 ");
	//TwAddVarCB(bar, "centerZ", TW_TYPE_FLOAT, setBasicShapeCenterZ, getBasicShapeCenterZ, &centerZ, "group = 'Center' step = 0.01 ");

	//color
	TwAddVarCB(bar, "colorR", TW_TYPE_FLOAT, setBasicShapeColorR, getBasicShapeColorR, &centerX, "group = 'Color' step = 0.05 ");
	TwAddVarCB(bar, "colorG", TW_TYPE_FLOAT, setBasicShapeColorG, getBasicShapeColorG, &centerY, "group = 'Color' step = 0.05 ");
	TwAddVarCB(bar, "colorB", TW_TYPE_FLOAT, setBasicShapeColorB, getBasicShapeColorB, &centerZ, "group = 'Color' step = 0.05 ");

	//add combobox for the layers
	TwEnumVal marblesEV[] = { { 0, "Line" },{ 1, "Comb" },{ 2, "Wavy" },{ 3, "Circular" },{4, "Vortex"} };
	TwType marbleType = TwDefineEnum("marbleType", marblesEV, 5);
	TwAddVarRW(bar, "MarbleSelection", marbleType, &selectedMarblingType, NULL);

	glutMainLoop();

	return 0;
}