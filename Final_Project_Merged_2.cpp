#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

//createArray function replaced by createBinaryFile()
//readBinary function eliminated, replaced by BinaryReader class
//All file operations now use filename parameter rather than hardcoded binary.dat

const int SIZE = 1000; // removed const int ARRAY_SIZE

// Modified writeBinary function with filename parameter
void writeBinary(string filename, int* values, int length) 
{
	ofstream outFile(filename, ios::binary); // Opens file in binary mode
	if (!outFile) // Checks if file opened successfully 
	{
		cerr << "Error opening file for writing: " << filename << endl;
		return;
	}
	outFile.write(reinterpret_cast<const char*>(&length), sizeof(int)); // Writes the length of the array first
	outFile.write(reinterpret_cast<const char*>(values), length * sizeof(int)); // Writes the entire array block to the file
	outFile.close(); // Closes the file
}

// New function to create array and write to binary file
void createBinaryFile(string filename) 
{
	int* arr = new int[SIZE];
	srand(time(0));
	for (int i = 0; i < SIZE; i++)  // initializes array with random numbers in the range [0, 999]
	{
		arr[i] = rand() % 1000;
	}
	writeBinary(filename, arr, SIZE); // calls writeBinary and writes array to file
	delete[] arr;
}


class BinaryReader 
{
private:
	int* values;
	int size;

	void readValues(const char* filename) // Reads binary and reinterprets as integers
	{
		ifstream inFile(filename, ios::binary);
		if (!inFile) 
		{
			cerr << "Error opening file for reading: " << filename << endl;
			values = nullptr;
			size = 0;
			return;
		}
		inFile.read(reinterpret_cast<char*>(&size), sizeof(int));
		values = new int[size];
		inFile.read(reinterpret_cast<char*>(values), size * sizeof(int));
		inFile.close();
	}

public:
	BinaryReader(const char* filename) : values(nullptr), size(0) 
	{
		readValues(filename);
	}

	~BinaryReader() // Destructor
	{
		delete[] values;
	}

	int* getValues() 
	{
		return values;
	}

	int getSize() 
	{
		return size;
	}
};


class Analyzer 
{
private:
	int* values;
	int size;

	int* cloneValues(int* src, int n) 
	{
		int* dest = new int[n];
		for (int i = 0; i < n; i++) 
		{
			dest[i] = src[i];
		}
		return dest;
	}

public:
	Analyzer(int* vals, int sz) : size(sz) 
	{
		values = cloneValues(vals, size);
	}

	~Analyzer() 
	{
		delete[] values;
	}

	string analyze() // Checks if array is empty and returns error. If data is present, calculates mean/min/max
	{
		if (size == 0) 
		{
			return "Array is empty.";
		}
		int minVal = values[0];
		int maxVal = values[0];
		double sum = 0;

		for (int i = 0; i < size; i++) 
		{
			if (values[i] < minVal) minVal = values[i];
			if (values[i] > maxVal) maxVal = values[i];
			sum += values[i];
		}

		double mean = sum / size;
		return "Mean: " + to_string(mean) + ", Min: " + to_string(minVal) + ", Max: " + to_string(maxVal);
	}
};


int main() 
{

	createBinaryFile("binary.dat");

	BinaryReader reader("binary.dat");

	Analyzer analyzer(reader.getValues(), reader.getSize());


	cout << "Array Analysis Results:" << endl;
	cout << analyzer.analyze() << endl;
	cout << "Values read from file:" << endl;
	
	for (int i = 0; i < reader.getSize(); ++i) // Loop to print each value
	{
		cout << reader.getValues()[i] << " ";
		
		if ((i + 1) % 25 == 0) // Adds a newline every 25 numbers for better formatting
		{
			cout << endl;
		}
	}

	return 0;
}
