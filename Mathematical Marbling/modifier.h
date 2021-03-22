#pragma once
#include <iostream>
#include "glm\glm.hpp"

using namespace std;

class Modifier {
private:
	int _type;	//0 - line

	//variables for each type

	//for line - 0
	glm::vec2 _lineDir;	//for line
	float _lineOmega, _lineLambda;
	glm::vec2 _lineStart;
	glm::vec2 _lineNormal;

	//for comb - 1
	glm::vec2 _combDir;
	float _combOmega, _combLambda;
	glm::vec2 _combStart;
	glm::vec2 _combNormal;
	float _combSpacing;

	//for wavy - 2
	float _wavyAmplitude,  _wavyWaveLength, _wavyPhase, _wavyAngle;

	//for Circular - 3
	glm::vec2 _circleCenter;
	float _circleRadius;
	float _circleOmega, _circleLambda;	//circle omega, circle lambda

	//for vortex - 4
	glm::vec2 _vortexCenter;
	float _vortexOmega;
	float _vortexLambda;

public:
	void setLine(glm::vec2 start, glm::vec2 dir) {
		_type = 0;
		_lineDir = glm::normalize(dir);
		_lineStart = start;
		_lineNormal = glm::normalize(glm::vec2(-_lineDir[1], _lineDir[0]));
		_lineOmega = 80;
		_lineLambda = 32;
	}

	void setComb(glm::vec2 start, glm::vec2 dir, float spacing) {
		_type = 1;
		_combDir = glm::normalize(dir);
		_combStart = start;
		_combNormal = glm::normalize(glm::vec2(-_combDir[1], _combDir[0]));
		_combOmega = 80;
		_combLambda = 10;
		_combSpacing = spacing;
	}

	void setWavy() {
		_type = 2;
		_wavyAmplitude = 25;
		_wavyWaveLength = 0.03;
		_wavyPhase = 0;
		_wavyAngle = 3.1415 / 4.0; 
	}

	void setCircular(glm::vec2 circleCenter, float circleRad) {
		_type = 3;
		_circleCenter = circleCenter;
		_circleRadius = circleRad;
		_circleOmega = 70;
		_circleLambda = 32;
	}

	void setVortex(glm::vec2 vortexCenter) {
		_type = 4;
		_vortexCenter = vortexCenter;
		_vortexOmega = 70;
		_vortexLambda = 32;
	}

	glm::vec2 doMarble(glm::vec2 currentPoint) {
		glm::vec2 newPoint;
		float d;
		if (_type == 0) //line
		{
			d = abs(glm::dot(currentPoint - _lineStart, _lineNormal));
			newPoint = currentPoint - ((_lineOmega * _lineLambda) / (d + _lineLambda)) * _lineDir;
		}
		else if (_type == 1) {
			d = abs(glm::dot(currentPoint - _combStart, _combNormal));
			float d2 = _combSpacing / 2 - abs(fmod(d, _combSpacing) - _combSpacing / 2);
			newPoint = currentPoint - ((_combOmega * _combLambda) / (d2 + _combLambda)) * _combDir;
		}
		else if (_type == 2) {
			float v = glm::dot(currentPoint, glm::vec2(sin(_wavyAngle), -cos(_wavyAngle)));
			float f = _wavyAmplitude * sin(_wavyWaveLength * v + _wavyPhase);
			newPoint = currentPoint + f * glm::vec2(cos(_wavyAngle), sin(_wavyAngle));
		}
		else if (_type == 3) {
			d = abs(glm::length(currentPoint - _circleCenter) - _circleRadius);
			float l = (_circleOmega * _circleLambda) / (d + _circleLambda);
			float theta = l / glm::length(currentPoint - _circleCenter);
			glm::mat2 rotMat = glm::mat2(cos(theta), sin(theta), -sin(theta), cos(theta));
			newPoint = _circleCenter + (currentPoint - _circleCenter) * rotMat;
		}
		else if (_type == 4) {
			d = glm::length(currentPoint - _vortexCenter);
			float l = (_vortexOmega * _vortexLambda) / (d + _vortexLambda);
			float theta = l / glm::length(currentPoint - _vortexCenter);
			glm::mat2 rotMat = glm::mat2(cos(theta), sin(theta), -sin(theta), cos(theta));
			newPoint = _vortexCenter + (currentPoint - _vortexCenter) * rotMat;
		}
		return newPoint;
	}
};