#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include <cmath>

using namespace std;

const int SIZE = 1000; // removed const int ARRAY_SIZE

//createArray function replaced by createBinaryFile()
//readBinary function eliminated, replaced by BinaryReader class
//All file operations now use filename parameter rather than hardcoded binary.dat

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
		ifstream inFile(filename, ios::binary); // Opens file in binary mode
        if (!inFile)
        {
            cerr << "Error opening file for reading: " << filename << endl;
            values = nullptr;
            size = 0;
            return;
        }
		inFile.read(reinterpret_cast<char*>(&size), sizeof(int)); // Reads the size of the array first
		values = new int[size]; // Allocates memory for the array
		inFile.read(reinterpret_cast<char*>(values), size * sizeof(int)); // Reads the entire array block from the file
        inFile.close();
    }

public:
	BinaryReader(const char* filename) : values(nullptr), size(0) // Constructor
    {
        readValues(filename);
    }

    ~BinaryReader() // Destructor
    {
        delete[] values;
    }

	int* getValues() // Getter for values
    {
        return values;
    }

	int getSize() // Getter for size
    {
        return size;
    }
};

// Base class : Analyzer Pure virtual class
class Analyzer
{
protected:
    int* values;
    int size;

	int* cloneValues(int* src, int n) // Clones the array to ensure each analyzer has its own copy
    {
        int* dest = new int[n];
        for (int i = 0; i < n; i++)
        {
            dest[i] = src[i];
        }
        return dest;
    }

public:
	Analyzer(int* vals, int sz) : size(sz) // Constructor
    {
        values = cloneValues(vals, size);
    }

	virtual ~Analyzer() // Virtual destructor
    {
        delete[] values;
    }

    virtual string analyze() = 0; // Pure virtual function
};

// Subclass 1: DuplicateAnalyzer
class DuplicateAnalyzer : public Analyzer // Inherits from Analyzer
{
public:
	DuplicateAnalyzer(int* vals, int sz) : Analyzer(vals, sz) {} // Constructor

	string analyze() override // Overrides analyze method
    {
		unordered_map<int, int> countMap; // Map to count occurrences of each value
        for (int i = 0; i < size; i++)
        {
            countMap[values[i]]++;
        }

		int duplicatesCount = 0; // Counts how many values appear more than once
        for (const auto& pair : countMap)
        {
            if (pair.second > 1)
            {
                duplicatesCount++;
            }
        }

        return "Number of duplicated values: " + to_string(duplicatesCount);
    }
};

// Subclass 2: MissingAnalyzer
class MissingAnalyzer : public Analyzer // Inherits from Analyzer
{
public:
	MissingAnalyzer(int* vals, int sz) : Analyzer(vals, sz) {} // Constructor

	string analyze() override // Overrides analyze method
    {
		unordered_set<int> valueSet(values, values + size); // Set to track existing values
        int missingCount = 0;

        // Checks for missing values in the range 0 to 999
        for (int i = 0; i < 1000; i++)
        {
            if (valueSet.find(i) == valueSet.end()) // If value is not in the set, it's missing
            {
                missingCount++;
            }
        }

		return "Number of missing values: " + to_string(missingCount); // Returns count of missing values
    }
};

// Subclass 3: StatisticsAnalyzer
class StatisticsAnalyzer : public Analyzer
{
public:
	StatisticsAnalyzer(int* vals, int sz) : Analyzer(vals, sz) {}// Constructor

	string analyze() override// Overrides analyze method to return computed statistics
    {
        if (size == 0)
        {
            return "Array is empty.";
        }

		int minVal = values[0];// Initialize min and max with the first element
        int maxVal = values[0];
        double sum = 0;

		for (int i = 0; i < size; i++) // Loop to calculate min, max, and sum
        {
            if (values[i] < minVal) minVal = values[i];
            if (values[i] > maxVal) maxVal = values[i];
            sum += values[i];
        }

		double mean = sum / size; // Calculates mean
        return "Mean: " + to_string(mean) + ", Min: " + to_string(minVal) + ", Max: " + to_string(maxVal);
    }
};

int main()
{
    createBinaryFile("binary.dat");

    BinaryReader reader("binary.dat");

    // Instantiate analyzers
    DuplicateAnalyzer duplicateAnalyzer(reader.getValues(), reader.getSize());
    MissingAnalyzer missingAnalyzer(reader.getValues(), reader.getSize());
    StatisticsAnalyzer statisticsAnalyzer(reader.getValues(), reader.getSize());

    cout << "Array Analysis Results:" << endl;
    cout << duplicateAnalyzer.analyze() << endl;
    cout << missingAnalyzer.analyze() << endl;
    cout << statisticsAnalyzer.analyze() << endl;

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
