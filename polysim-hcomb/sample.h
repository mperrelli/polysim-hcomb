///////////////////////////////////////////////////////////////////////
//*                                                                 *//
//*  File:      sample.h                                            *//
//*  Author:    Matt Perrelli                                       *//
//*                                                                 *//
//*  Header file for the sample class.                              *//
//*                                                                 *//
///////////////////////////////////////////////////////////////////////
#pragma once

// Defines the maximum number of beads the simulation can use.
const int MAX_BEADS = 10000;

// A type to hold the head information for all the arms in the H-Comb
struct coordinate{
	int x;
	int y;
};

class sample
{
private:
	int beadsX[MAX_BEADS];
	int beadsY[MAX_BEADS];
	int beadCount, currentArm;
	coordinate arm1Head, arm2Head, arm3Head, arm4Head, arm5Head;
	bool buildExtraArms;

	double XCM, YCM, tensor11, tensor12, tensor22, lamda1, lamda2, 
		asphericity, radiusofGyration;

	void runCalculations();

	// Calculation functions
	double calculateXCM();
	double calculateYCM();
	double calculateTensor11();
	double calculateTensor12();
	double calculateTensor22();
	double calculateLamda1();
	double calculateLamda2();
	double calculateAsphericity();
	double calculateRadiusofGyration();

public:
	// Constructor and destructor
	sample(void);
	~sample(void);

	// Functionality
	void addBead();
	void addBeads(int amount);
	void advanceCurrentArm();
	void growArm(int x, int y, int currentArm);
	void setHead(int x, int y, int currentArm);
	void setExtraArmParams();

	// Gets and sets
	int getBeadCount();
	double getXCM();
	double getYCM();
	double getTensor11();
	double getTensor12();
	double getTensor22();
	double getLamda1();
	double getLamda2();
	double getAsphericity();
	double getRadiusofGyration();
};
