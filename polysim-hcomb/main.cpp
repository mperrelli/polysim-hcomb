///////////////////////////////////////////////////////////////////////
//*                                                                 *//
//*  PolySim                                                        *//
//*                                                                 *//
//*  Modeling and simulation of polymers in two dimensions.         *//
//*                                                                 *// 
//*  File:      main.cpp                                            *//
//*  Author:    Matt Perrelli                                       *//
//*                                                                 *//
//*  This is the main file that the program runs through. All input *//
//*  data is collected here and pushed to the appropriate manager.  *//
//*  Data is then output through this file.                         *//
//*                                                                 *//
///////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;
#include "sample.h"

// Prototypes
void outputHistogramData(int bins, int sampleAmt, sample* s[], int data);
double getMaxof(int data, sample* s[], int sampleAmt);

// Constants
const int MAX_SAMPLES = 20000;

const int ASPHERICITY = 1;
const int RADIUSOFGYRATION = 2;
const int LAMDA1 = 4;
const int LAMDA2 = 5;

///////////////////////////////////////////////////////////////////////
//*                                                                 *//
//*  Function:  Main                                                *//
//*                                                                 *//
///////////////////////////////////////////////////////////////////////
//// Variables ////////////////////////////////////////////////////////
//                                                                   //
//  samples:            The array that holds pointers to sample      //
//                      objects                                      //
//                                                                   //
//  outputFile:         output datafile stream                       //
//                                                                   //
//  beadAmt:            Bead amount                                  //
//                                                                   //
//  sampleAmt:          sample amount                                //
//                                                                   //
//  sum,sum2,sum3,sum4: temporary variables                          //
//                                                                   //
///////////////////////////////////////////////////////////////////////
int main()
{
	// Defines a list of pointers to instances of the sample class
	sample* samples[MAX_SAMPLES];

	// Output file stream
	ofstream outputFile;

	// Variables
	int beadAmt, sampleAmt, pause;

	// Temporary variables
	double sum = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0;

	// final avg vars
	double avgSquareEndToEndDist = 0.0,
		   avgSquareEndToEndDistSq = 0.0,
		   avgLamda1 = 0.0,
		   avgLamda1Sq = 0.0,
		   avgLamda2 = 0.0,
		   avgLamda2Sq = 0.0,
		   avgAsphericity = 0.0,
		   avgAsphericitySq = 0.0,
		   avgRadiusOfGyration = 0.0,
		   avgRadiusOfGyrationSq = 0.0;

	// final standard deviations
	double sdSquareEndToEndDist = 0.0,
		   sdLamda1 = 0.0,
		   sdLamda2 = 0.0,
		   sdAsphericity = 0.0,
		   sdRadiusOfGyration = 0.0;

	// Set format
	cout.setf(ios::fixed);
	outputFile.setf(ios::fixed);

	// User input
	cout << "Bead Amount: ";
	cin >> beadAmt;
	cout << "Sample Amount: ";
	cin >> sampleAmt;

	// Builds the samples
	for(int i = 0; i < sampleAmt; i++)
	{
		sample* p = new sample();
		samples[i] = p;
		samples[i]->addBeads(beadAmt - 1);
	}

	// Gets average lamda1 and lamda2
	for(int i = 0; i < sampleAmt; i++)
	{
		sum += samples[i]->getLamda1();
		sum2 += pow(samples[i]->getLamda1(), 2.0);
		sum3 += samples[i]->getLamda2();
		sum4 += pow(samples[i]->getLamda2(), 2.0);
	}

	avgLamda1 = sum / sampleAmt;
	avgLamda1Sq = sum2 / sampleAmt;
	avgLamda2 = sum3 / sampleAmt;
	avgLamda2Sq = sum4 / sampleAmt;

	// Standard deviation of lamda1 and lamda2
	sdLamda1 = sqrt((avgLamda1Sq - avgLamda1 * avgLamda1) 
			   / (sampleAmt - 1));
	sdLamda2 = sqrt((avgLamda2Sq - avgLamda2 * avgLamda2) 
		       / (sampleAmt - 1));

	// Reset sums for use again
	sum = 0.0;
	sum2 = 0.0;
	sum3 = 0.0;
	sum4 = 0.0;

	// gets average asphericity and radius of gyration
	for(int i = 0; i < sampleAmt; i++)
	{
		sum += samples[i]->getAsphericity();
		sum2 += pow(samples[i]->getAsphericity(), 2.0);
		sum3 += samples[i]->getRadiusofGyration();
		sum4 += pow(samples[i]->getRadiusofGyration(), 2.0);
	}

	avgAsphericitySq = sum2 / sampleAmt;
	avgAsphericity = sum / sampleAmt;
	avgRadiusOfGyrationSq = sum4 / sampleAmt;
	avgRadiusOfGyration = sum3 / sampleAmt;

	// Gets the standard deviation of the mean of asphericity and radius of
	// gyration
	sdAsphericity = sqrt((avgAsphericitySq - avgAsphericity 
		            * avgAsphericity) / (sampleAmt - 1));
	sdRadiusOfGyration = sqrt((avgRadiusOfGyrationSq - 
				         avgRadiusOfGyration * avgRadiusOfGyration) 
						 / (sampleAmt - 1));

	// Output data to screen
	cout << endl;
	cout << "Beads: " << beadAmt << endl;
	cout << "Samples: " << sampleAmt << endl;
	cout << "\n\nQuantity" << setw(13) << "Average" << setw(27) 
		 << "Standard Deviation\n";
	cout << "-----------------------------------------------\n";
	cout << "Lamda1  " << setw(15) << setprecision(6) << avgLamda1 
		               << setw(18) << setprecision(6) << sdLamda1 
					   << endl;
	cout << "Lamda2  " << setw(15) << setprecision(6) << avgLamda2 
					   << setw(18) << setprecision(6) << sdLamda2 
					   << endl;
	cout << "s^2     " << setw(15) << setprecision(6) 
					   << avgRadiusOfGyration 
		               << setw(18) << setprecision(6) 
					   << sdRadiusOfGyration << endl;
	cout << "A       " << setw(15) << setprecision(6) << avgAsphericity 
		               << setw(18) << setprecision(6) << sdAsphericity 
					   << endl;

	// Build output
	outputFile.open("output.txt");

	if(outputFile.fail())
	{
		cout << "Failed to open output file.\n";
		exit(1);
	}

	outputFile << "Beads: " << beadAmt << endl;
	outputFile << "Samples: " << sampleAmt << endl;

	outputFile << "\n\nQuantity" << setw(13) << "Average" << setw(27) 
		 << "Standard Deviation\n";
	outputFile << "-----------------------------------------------\n";
	outputFile << "Lamda1  " << setw(15) << setprecision(6) << avgLamda1 
		               << setw(18) << setprecision(6) << sdLamda1 
					   << endl;
	outputFile << "Lamda2  " << setw(15) << setprecision(6) << avgLamda2 
					   << setw(18) << setprecision(6) << sdLamda2 
					   << endl;
	outputFile << "s^2     " << setw(15) << setprecision(6) 
					   << avgRadiusOfGyration 
		               << setw(18) << setprecision(6) 
					   << sdRadiusOfGyration << endl;
	outputFile << "A       " << setw(15) << setprecision(6) << avgAsphericity 
		               << setw(18) << setprecision(6) << sdAsphericity 
					   << endl << endl;

	outputFile << "Lamda1" << "\t" << "Lamda2" << "\t" 
		       << "s^2" << "\t" << "A\n";

	for(int i = 0; i < sampleAmt; i++)
	{
		outputFile << samples[i]->getLamda1() << "\t" 
			       << setprecision(6) 
			       << samples[i]->getLamda2() << "\t" 
				   << setprecision(6) 
				   << samples[i]->getRadiusofGyration() << "\t" 
				   << setprecision(6) 
				   << samples[i]->getAsphericity() << "\t" 
				   << endl;
	}

	outputFile.close();

	// Output the histrogram data for all quantities
	outputHistogramData(20, sampleAmt, samples, RADIUSOFGYRATION);
	outputHistogramData(20, sampleAmt, samples, ASPHERICITY);
	outputHistogramData(20, sampleAmt, samples, LAMDA1);
	outputHistogramData(20, sampleAmt, samples, LAMDA2);

	// Wait
	cin >> pause;

	return 0;
}

///////////////////////////////////////////////////////////////////////
//*                                                                 *//
//*  Function:  outputHistogramData                                 *//
//*                                                                 *//
//*  Description:  Outputs a datafile for construction a histogram. *//
//*                                                                 *//
///////////////////////////////////////////////////////////////////////
//// Variables ////////////////////////////////////////////////////////
//                                                                   //
//  MAX_BINS:     max amount of bins                                 //
//                                                                   //
//  binSize:      bin size                                           //
//                                                                   //
//  temp:         temporary variable, hold asphericity               //
//                                                                   //
//  histInfoFile: output file stream                                 //
//                                                                   //
//  range:        array that holds the upper bounds for each bin     //
//                                                                   //
//  count:        array that holds the count for each bin            //
//                                                                   //
///////////////////////////////////////////////////////////////////////
void outputHistogramData(int bins, int sampleAmt, sample* s[], int data)
{
	const int MAX_BINS = 35;
	double binSize = getMaxof(data, s, sampleAmt) / bins;
	double temp;

	// Output file stream
	ofstream histInfoFile;

	double range[MAX_BINS];
	int count[MAX_BINS];

	// Initialize arrays
	for(int i = 1; i <= bins; i++)
	{
		count[i - 1] = 0;
		range[i - 1] = (double)i * binSize;
	}

	// Build histogram data
	switch(data)
	{
	case ASPHERICITY:

		for(int i = 0; i < sampleAmt; i++)
		{
			temp = s[i]->getAsphericity();

			for(int j = 0; j < bins; j++)
			{
				if(temp <= range[j])
				{
					count[j]++;
					break;
				}
			}
		}

		histInfoFile.open("AsphericityHistData.txt");

		break;
	case RADIUSOFGYRATION:

		for(int i = 0; i < sampleAmt; i++)
		{
			temp = s[i]->getRadiusofGyration();

			for(int j = 0; j < bins; j++)
			{
				if(temp <= range[j])
				{
					count[j]++;
					break;
				}
			}
		}

		histInfoFile.open("ROGHistData.txt");

		break;

	case LAMDA1:

		for(int i = 0; i < sampleAmt; i++)
		{
			temp = s[i]->getLamda1();

			for(int j = 0; j < bins; j++)
			{
				if(temp <= range[j])
				{
					count[j]++;
					break;
				}
			}
		}

		histInfoFile.open("Lamda1HistData.txt");

		break;

	case LAMDA2:

		for(int i = 0; i < sampleAmt; i++)
		{
			temp = s[i]->getLamda2();

			for(int j = 0; j < bins; j++)
			{
				if(temp <= range[j])
				{
					count[j]++;
					break;
				}
			}
		}

		histInfoFile.open("Lamda2HistData.txt");

		break;
	}

	// Build output

	if(histInfoFile.fail())
	{
		cout << "Failed to open histogram file.\n";
		exit(1);
	}

	histInfoFile << "Bin\tFrequency\n";

	for(int i = 0; i < bins; i++)
	{
		histInfoFile << range[i] << "\t" << count[i] << endl;
	}

	histInfoFile.close();
}

double getMaxof(int data, sample* s[], int sampleAmt)
{
	double max = 0;

	switch(data)
	{
	case ASPHERICITY:

		for(int i = 0; i < sampleAmt; i++)
		{
			if(s[i]->getAsphericity() > max)
			{
				max = s[i]->getAsphericity();
			}
		}

		break;
	case RADIUSOFGYRATION:

		for(int i = 0; i < sampleAmt; i++)
		{
			if(s[i]->getRadiusofGyration() > max)
			{
				max = s[i]->getRadiusofGyration();
			}
		}

		break;

	case LAMDA1:

		for(int i = 0; i < sampleAmt; i++)
		{
			if(s[i]->getLamda1() > max)
			{
				max = s[i]->getLamda1();
			}
		}

		break;

	case LAMDA2:

		for(int i = 0; i < sampleAmt; i++)
		{
			if(s[i]->getLamda2() > max)
			{
				max = s[i]->getLamda2();
			}
		}

		break;
	}

	return max;
}
