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
	currentArm = 1;
	beadCount++;	
	buildExtraArms = false;
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

	switch(currentArm)
	{
	case 1:
		// Grow arm1
		y = arm1Head.y;
		x = arm1Head.x;
		break;

	case 2:
		// Grow arm2
		y = arm2Head.y;
		x = arm2Head.x;
		break;

	case 3:
		// Grow arm3
		y = arm3Head.y;
		x = arm3Head.x;
		break;

	case 4:
		y = arm4Head.y;
		x = arm4Head.x;
		break;

	case 5:
		y = arm5Head.y;
		x = arm5Head.x;
		break;

	default:;

	}

	growArm(x, y, currentArm);
	advanceCurrentArm();
}

// Grows a given arm in a random direction
//
void sample::growArm(int x, int y, int currentArm)
{
	int growDirection = rand() % 4 + 1;

	switch(growDirection)
	{
	case 1:
		// Grow North
		y = y + 1;
		break;

	case 2:
		// Grow South
		y = y - 1;
		break;

	case 3:
		// Grow East
		x = x + 1;
		break;

	case 4:
		// Grow West
		x = x - 1;
		break;

	default:;

	}

	setHead(x, y, currentArm);
	beadsX[beadCount] = x;
	beadsY[beadCount] = y;
	cout << "Arm " << currentArm << ": (" << x << ", " << y << ")" << endl;
	beadCount++;
}

// Updates the head bead of the current arm
//
void sample::setHead(int x, int y, int currentArm)
{
	switch(currentArm)
	{
	case 1:
		// Grow arm1
		arm1Head.y = y;
		arm1Head.x = x;
		break;

	case 2:
		// Grow arm2
		arm2Head.y = y;
		arm2Head.x = x;
		break;

	case 3:
		// Grow arm3
		arm3Head.y = y;
		arm3Head.x = x;
		break;

	case 4:
		// Grow arm4
		arm4Head.y = y;
		arm4Head.x = x;
		break;

	case 5:
		// Grow arm5
		arm5Head.y = y;
		arm5Head.x = x;
		break;

	default:;

	}
}
// A conveniance function to add multiple beads at once
//
void sample::addBeads(int amount)
{
	int armLength = amount / 5;

	// Adds beads for the star portion of the H-Comb
	for(int i = 0; i < amount - (armLength * 2); i++)
	{
		addBead();
	}

	// Switch the build parameters
	setExtraArmParams();

	// Adds the beads for the additional two arms to make 
	// the star an H-Comb
	for(int i = 0; i < (armLength * 2); i++)
	{
		addBead();
	}

	runCalculations();
}

// Sets the necessary parameters for when we switch from
// building the star polymer to the H-Comb.
//
void sample::setExtraArmParams()
{
	// Switch the current arm to 4.
	currentArm = 4;

	// Set the heads of the two arms to the endpoint of one
	// of the ends of the star we have constructed. I chose 
	// 3 but any can be used.
	arm4Head.y = arm3Head.y;
	arm4Head.x = arm3Head.x;
	arm5Head.y = arm3Head.y;
	arm5Head.x = arm3Head.x;
}

// Checks the current arm and moves it to the next arm
// to be added to. 1, 2 and 3 will rotate amoung one another.
// Arms 4 and 5 will rotate between one another.
//
void sample::advanceCurrentArm()
{
	switch(currentArm)
	{
	case 1:
		currentArm = 2;
		break;

	case 2: 
		currentArm = 3;
		break;

	case 3:
		currentArm = 1;
		break;

	case 4:
		currentArm = 5;
		break;

	case 5:
		currentArm = 4;
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