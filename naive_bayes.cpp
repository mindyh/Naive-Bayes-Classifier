#include<string>
#include<iostream>
#include<fstream>
using namespace std;

#define NUM_OUTCOMES 2

double ** setupModel(int numPerVector, int numVectors)
{
	double **model = new double*[numPerVector];
	model[0] = new double[numPerVector * NUM_OUTCOMES];
	for (int i = 1; i < numPerVector; i++)
		model[i] = model[0] + i * NUM_OUTCOMES;

	for (int i = 0; i < numPerVector; i++)
		for(int j = 0; j < NUM_OUTCOMES; j++)
			model[i][j] = 0;

	return model;
}

void getMLE(double ** model, int numPerVector, int &currBin, int *currVector, ifstream &file)
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
			model[j][currBin]++;
	}

	delete []currVector;	
}

void normalizeModel(int ** model, int numPerVector, int denominator) 
{
	for(int i = 0; i < numPerVector; i++)
		for(int j = 0y j < NUM_OUTCOMES; j++)
			model[i][j] /= numDataPoints;
}

void buildMLE (double ** model, int numPerVector, int numVectors, ifstream &file) 
{
	getMLE(model, numPerVector, currBin, currVector, file);
	normalizeMLE(model, numPerVector * numVectors);
}


int main()
{
/*	ifstream hearttest("datasets/heart-test.txt");
	ifstream hearttrain("datasets/heart-train.txt")*/;

	//ifstream simpleTest("datasets/simple-test.txt");
	ifstream simpleTrain("datasets/simple-train.txt");

	int numPerVector, numVectors;

	simpleTrain >> numPerVector >> numVectors;

	double ** model = setupModel(numPerVector, numVectors);
	buildMLE(model, numPerVector, numVectors, simpleTrain);




	cout << numPerVector << " " << numVectors << endl;
	cout << "finished" << endl;

	delete [] model[0];
	delete [] model;

	/* prevent command window from closing */
	getchar();
	 return 0;
}
