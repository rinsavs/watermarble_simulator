#pragma once
#include <iostream>
#include <math.h>
#include "glm\glm.hpp"

using namespace std;

class BasicShape {
private:
	string _name;
	float _radius;
	glm::vec2 _center;
	int _layerID;
	glm::vec3 _layerColor;

public:
	BasicShape() {}
	void set(string name, float rad, glm::vec2 center, int layID, glm::vec3 color) {
		_name = name;
		_radius = rad;
		_center = center;
		_layerID = layID;
		_layerColor = color;
	}

	void setRadius(float r) {
		_radius = r;
	}

	void setCenter(const float *c) {
		_center[0] = c[0];
		_center[1] = c[1];
		//_center[2] = c[2];
	}

	void setCenterX(const float c) {
		_center[0] = c;
	}

	void setCenterY(const float c) {
		_center[1] = c;
	}

	/*void setCenterZ(const float c) {
		_center[2] = c;
	}*/

	void setColor(const float *col) {
		_layerColor = glm::vec3(col[0], col[1], col[2]);
	}

	void setColorR(const float c) {
		_layerColor[0] = c;
	}

	void setColorG(const float c) {
		_layerColor[1] = c;
	}

	void setColorB(const float c) {
		_layerColor[2] = c;
	}
	
	string getName() {
		return _name;
	}

	float getRadius() {
		return _radius;
	}

	glm::vec2 getCenter() {
		return _center;
	}

	float getCenterX() {
		return _center[0];
	}

	float getCenterY() {
		return _center[1];
	}

	float getCenterZ() {
		return _center[2];
	}

	int getLayID() {
		return _layerID;
	}

	glm::vec3 getColor(){
		return _layerColor;
	}

	float getColorR()	{return _layerColor[0];}
	float getColorG()	{return _layerColor[1];}
	float getColorB()	{return _layerColor[2];}
};