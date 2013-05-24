#include<string>
#include<vector>
#include<iostream>
#include<fstream>
using namespace std;

#define NUM_CLASSIFICATIONS 2
#define NUM_INDICATOR_OUTCOMES 2

vector<double **> setupModel(int numVariables)
{
	vector<double **> model(numVariables);

	for(int i = 0; i < numVariables; i++)
	{
		double **x = new double*[NUM_INDICATOR_OUTCOMES];
		x[0] = new double[NUM_INDICATOR_OUTCOMES * NUM_CLASSIFICATIONS];
		
		for (int j = 0; j < NUM_INDICATOR_OUTCOMES; i++) 
			x[j] = x[0] + j * NUM_CLASSIFICATIONS;

		model[i] = x;
	}

	return model;
}

void getMLE(vector<double **> model, int numPerVector, int numVectors, ifstream &file, double *outcomeVector)
{
	int currBin;
	int *currVector = new int[numPerVector];

	for(int i = 0; i < numVectors; i++)
	{
		/* get the vector */
		for(int j = 0; j < numPerVector; j++)
			file >> currVector[j];

		/* get the binary output of the vector */
		string line;
		getline(file, line);
		currBin = line[line.length() - 1] - '0'; // last character on the line, guaranteed. Convert from char to int
			
		/* add the vector to the model */
		for(int j = 0; j < numPerVector; j++)
			model[j][currVector[j]][currBin]++;

		outcomeVector[currBin]++;
	}

	delete []currVector;	
}

void normalizeVectors(vector<double **> model, double *outcomeVector, int numVariables, int denominator) 
{
	/* Normalize model vectors */
	for(int i = 0; i < numVariables; i++)
		for(int j = 0; j < NUM_INDICATOR_OUTCOMES; j++)
			for(int k = 0; k < NUM_CLASSIFICATIONS; k++)
				model[i][j][k] /= denominator;

	/* Normalize outcome vector */
	for(int i = 0; i < NUM_CLASSIFICATIONS; i++)
		outcomeVector[i] /= denominator;
}

void buildMLE (vector<double **> model, int numVariables, int numVectors, ifstream &file, double *outcomeVector) 
{
	getMLE(model, numVariables, numVectors, file, outcomeVector);
	normalizeVectors(model, outcomeVector, numVariables, numVariables*numVectors);
}

void initOutcomeVector(double *vector)
{
	for(int i = 0; i < NUM_CLASSIFICATIONS; i++) vector[i] = 0;
}

void initModel(vector<double **> model, int numVariables)
{
	for (int i = 0; i < numVariables; i++)
		for(int j = 0; j < NUM_INDICATOR_OUTCOMES; j++)
			for(int k = 0; k < NUM_CLASSIFICATIONS; k++)
				model[i][j][k] = 0;
}

void cleanupModel(vector<double **> model, int numVariables)
{
	for(int i = 0; i < numVariables; i++)
	{
		delete [] model[i][0];
		delete [] model[i];
	}
}

int main()
{
/*	ifstream hearttest("datasets/heart-test.txt");
	ifstream hearttrain("datasets/heart-train.txt")*/;

	ifstream simpleTest("datasets/simple-test.txt");
	ifstream simpleTrain("datasets/simple-train.txt");

	int numVariables, numVectors;

	simpleTrain >> numVariables >> numVectors;

	double outcomeVector[NUM_CLASSIFICATIONS];
	initOutcomeVector(outcomeVector);
	
	vector<double **> model = setupModel(numVariables);
	initModel(model, numVariables);
	buildMLE(model, numVariables, numVectors, simpleTrain, outcomeVector);
 
	//testMLE(model,
	


	cout << numVariables << " " << numVectors << endl;
	cout << "finished" << endl;

	cleanupModel(model, numVariables);

	/* prevent command window from closing */
	getchar();
	 return 0;
}
