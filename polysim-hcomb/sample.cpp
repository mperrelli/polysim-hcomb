///////////////////////////////////////////////////////////////////////
//*                                                                 *//
//*  File:      sample.cpp                                          *//
//*  Author:    Matt Perrelli                                       *//
//*                                                                 *//
//*  This is the implementation code for the sample class. This     *//
//*  holds all the basic functionality for managing H-Comb          *//
//*  samples.                                                       *//
//*                                                                 *//
///////////////////////////////////////////////////////////////////////
#include <iostream>
#include <stdlib.h>
#include <math.h>
using namespace std;
#include "sample.h"


// Constructor for a sample. When a sample is made it is
// automatically given an initial bead at coordinates 0,0
//
sample::sample(void)
{
	beadCount = 0;
	beadsX[beadCount] = 0;
	beadsY[beadCount] = 0;
	arm1Head.x = 0;
	arm1Head.y = 0;
	arm2Head.x = 0;
	arm2Head.y = 0;
	arm3Head.x = 0;
	arm3Head.y = 0;
	growDirection = 1;
	beadCount++;	
}

// Destructor for a sample.
//
sample::~sample(void)
{
	
}

// Adds a bead to the end of an arm in a sample.
// This is done by selecting the smallest arm and adding to it
// so as to keep the arms the same length.
//
void sample::addBead()
{
	int x, y;

	switch(growDirection)
	{
	case 1:
		// Grow arm1:y++
		y = arm1Head.y + 1;
		x = arm1Head.x;
		arm1Head.y = y;
		arm1Head.x = x;
		break;

	case 2:
		// Grow arm2:x++
		y = arm2Head.y;
		x = arm2Head.x + 1;
		arm2Head.y = y;
		arm2Head.x = x;
		break;

	case 3:
		// Grow arm3:y--
		y = arm3Head.y - 1;
		x = arm3Head.x;
		arm3Head.y = y;
		arm3Head.x = x;
		break;

	default:;

	}

	beadsX[beadCount] = x;
	beadsY[beadCount] = y;
	beadCount++;
	cout << x << " " << y << endl;
	advanceGrowDirection();
}
// A conveniance function to add multiple beads at once
//
void sample::addBeads(int amount)
{
	for(int i = 0; i < amount; i++)
	{
		addBead();
	}

	runCalculations();
}

void sample::advanceGrowDirection()
{
	switch(growDirection)
	{
	case 1:
		growDirection = 2;
		break;

	case 2: 
		growDirection = 3;
		break;

	case 3:
		growDirection = 1;
		break;

	default: ;
	}
}

// Returns the next available bead location in the array of beads
//
int sample::getBeadCount()
{
	return beadCount;
}

// Returns the Squared end to end distance. Calculated by running
// the coordinate distance formula on the first and last beads
// in a sample.
//
double sample::calculateSquareEndToEndDist()
{
	int x1 = beadsX[0];
	int y1 = beadsY[0];
	int x2 = beadsX[beadCount - 1];
	int y2 = beadsY[beadCount - 1];

	double distance = ((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1));

	return distance;
}

// Returns average X coordinate across all beads
// Shows us the X coordinate for the center of mass
// of our sample.
//
double sample::calculateXCM()
{
	double sum = 0.0;

	for(int i = 0; i < beadCount; i++)
	{
		sum += beadsX[i];
	}

	sum = sum / beadCount;

	return sum;
}

// Returns average Y coordinate accross all beads
// Shows us the Y coordinate for the center of mass
// of our sample.
//
double sample::calculateYCM()
{
	double sum = 0.0;

	for(int i = 0; i < beadCount; i++)
	{
		sum += beadsY[i];
	}

	sum = sum / beadCount;

	return sum;
}

// Returns the tensor for matrix position 1,1
//
double sample::calculateTensor11()
{
	double sum = 0.0;

	for(int i = 0; i < beadCount; i++)
	{
		sum += pow((beadsX[i] - XCM), 2.0);
	}

	sum = sum / beadCount;

	return sum;
}

// Returns the tensor for matrix position 1,2
// Same as Tensor21
//
double sample::calculateTensor12()
{
	double sum = 0.0;

	for(int i = 0; i < beadCount; i++)
	{
		sum += ((beadsX[i] - XCM) * (beadsY[i] - YCM));
	}

	sum = sum / beadCount;

	return sum;
}

// Returns the tensor for matrix position 2,2
//
double sample::calculateTensor22()
{
	double sum = 0.0;

	for(int i = 0; i < beadCount; i++)
	{
		sum += pow((beadsY[i] - YCM), 2.0);
	}

	sum = sum / beadCount;

	return sum;
}

// Returns Lamda1 which is the distance from the center of mass to the
// encompassing ellipse on the major axis around the sample
//
double sample::calculateLamda1()
{
	double a = tensor11, b = tensor12, c = tensor22, lamda, factor;

	factor = (1.0 / 2) * sqrt((a*a) - (2 * a * c) + (c * c) 
		     + ((4 * (b * b))));

	lamda = ((a + c) / 2) + factor;

	return lamda;
}

// Returns Lamda2 which is the distance from the center of mass to the
// encompassing ellipse on the minor axis around the sample
//
double sample::calculateLamda2()
{
	double a = tensor11, b = tensor12, c = tensor22, lamda, factor;

	factor = (1.0 / 2) * sqrt((a*a) - (2 * a * c) + (c * c) 
		     + ((4 * (b * b))));

	lamda = ((a + c) / 2) - factor;

	return lamda;
}

// Returns the Asphericity which is the measure of how round a polymer is.
//
double sample::calculateAsphericity()
{
	double asphericity;

	asphericity = pow((lamda1 - lamda2), 2.0) 
		          / pow((lamda2 + lamda1), 2.0);

	return asphericity;
}

double sample::calculateRadiusofGyration()
{
	double rog;

	rog = lamda2 + lamda1;

	return rog;
}

// Runs all the necessary calculations on the sample and stores them.
//
void sample::runCalculations()
{
	squareEndToEndDist = calculateSquareEndToEndDist();
	XCM = calculateXCM();
	YCM = calculateYCM();
	tensor11 = calculateTensor11();
	tensor12 = calculateTensor12();
	tensor22 = calculateTensor22();

	// Stores the lamdas in temp variables
	double temp1 = calculateLamda1();
	double temp2 = calculateLamda2();

	// Puts the greater lamda in lamda1 and the lesser in lamda2
	if(temp1 > temp2)
	{
		lamda1 = temp1;
		lamda2 = temp2;
	}
	else
	{
		lamda1 = temp2;
		lamda2 = temp1;
	}

	asphericity = calculateAsphericity();
	radiusofGyration = calculateRadiusofGyration();
}

// Below this point are all get functions
double sample::getSquareEndToEndDist()
{
	return squareEndToEndDist;
}

double sample::getXCM()
{
	return XCM;
}

double sample::getYCM()
{
	return YCM;
}

double sample::getTensor11()
{
	return tensor11;
}

double sample::getTensor12()
{
	return tensor12;
}

double sample::getTensor22()
{
	return tensor22;
}

double sample::getLamda1()
{
	return lamda1;
}

double sample::getLamda2()
{
	return lamda2;
}
double sample::getAsphericity()
{
	return asphericity;
}

double sample::getRadiusofGyration()
{
	return radiusofGyration;
}