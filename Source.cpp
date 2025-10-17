#include <iostream> 
#include <fstream>  
#include <cstdlib>  
#include <ctime>    

using namespace std;

// Defines the array size as a constant outside the functions
const int ARRAY_SIZE = 1000;
// createArray creates a dynamic array of 1000 integers.
// The length is passed by pointer and updated within the function.
int* createArray(int* length) 
{
    // Allocates memory for ARRAY_SIZE integers on the heap
    int* dynamicArray = new int[ARRAY_SIZE];
    // Initializes the array with random numbers in the range [0, 999]
    for (int i = 0; i < ARRAY_SIZE; ++i) 
    {
        dynamicArray[i] = rand() % 1000;
    }
    // Assigns the array size to the caller's length variable via the pointer
    *length = ARRAY_SIZE;
    // Return the pointer to the newly created array
    return dynamicArray;
}
// writeBinary writes the array length and its contents to a binary file.
void writeBinary(int* values, int length) 
{
    // Opens the file "binary.dat" in binary output mode
    ofstream outFile("binary.dat", ios::binary);
    // Checks if the file was opened successfully
    if (!outFile.is_open()) 
    {
        cerr << "Error: Could not open binary.dat for writing." << endl;
        return;
    }
    // Writes the length of the array first
    outFile.write(reinterpret_cast<const char*>(&length), sizeof(length));
    // Writes the entire array block to the file
    outFile.write(reinterpret_cast<const char*>(values), length * sizeof(int));
    // Closes the file
    outFile.close();
}
// Function to read array data from a binary file.
// The length is passed by reference and updated within the function.
int* readBinary(int& length) 
{
    // Opens the file "binary.dat" in binary input mode
    std::ifstream inFile("binary.dat", ios::binary);
    // Checks if the file was opened successfully
    if (!inFile.is_open()) 
    {
        cerr << "Error: Could not open binary.dat for reading." << endl;
        length = 0; // Sets length to 0 to indicate failure
        return nullptr; // Returns null pointer on failure
    }
    // Reads the first value from the file, which is the array length
    inFile.read(reinterpret_cast<char*>(&length), sizeof(length));
    // Creates a new dynamic array using the length read from the file
    int* readArray = new int[length];
    // Reads the block of integer data from the file into the new array
    inFile.read(reinterpret_cast<char*>(readArray), length * sizeof(int));
    // Close the file
    inFile.close();
    // Returns a pointer to the newly created array
    return readArray;
}
// Main function to orchestrate the program flow
int main() {
    // Seeds the random number generator once at the start of the program
    srand(static_cast<unsigned int>(time(0)));
    // --- Part 1: Creates array, writes to file, and deletes ---
    int originalLength = 0;
    // Calls createArray, passing the address of originalLength to be updated
    int* originalArray = createArray(&originalLength);
    cout << "Array of length " << originalLength << " created." << endl;
    // Calls writeBinary to save the array to a file
    writeBinary(originalArray, originalLength);
    cout << "Array written to binary.dat." << endl;
    // Deletes the dynamically created array and sets the pointer to null
    delete[] originalArray;
    originalArray = nullptr;
    cout << "Original array deleted." << std::endl << endl;
    // --- Part 2: Reads from file, prints, and deletes ---
    int readLength = 0;
    // Calls readBinary, passing readLength by reference to be updated
    int* readArray = readBinary(readLength);
    // Checks if the array was read successfully
    if (readArray != nullptr) {
        cout << "Array of length " << readLength << " read from binary.dat." << endl;
        cout << "Values read from file:" << endl;
        // Loop to print each value in the newly read array
        for (int i = 0; i < readLength; ++i) 
        {
            cout << readArray[i] << " ";
            // Add a newline every 20 numbers for better formatting
            if ((i + 1) % 20 == 0) 
            {
                cout << endl;
            }
        }
        cout << std::endl << endl;
        // Deletes the second dynamically created array and sets the pointer to null
        delete[] readArray;
        readArray = nullptr;
        cout << "Read array deleted." << endl;
    }
    else 
    {
        cerr << "Program failed to read array from file." << endl;
        return 1; // Indicate an error
    }
    // Terminate the program successfully
    return 0;
}
