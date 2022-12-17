#include <fstream>
#include "mpi.h"
#include "mpi_timer.h"
#include "vigenere_cipher.h"
#include "matrix_helper.h"

#define PREPROCESSING_PATTERN "([[:punct:]]|\\s|[0-9])"

/*
 * Vegenere encryption and decryption using MPI
 */
void readWord(string &wordToEncrypt);
void readKeyword(string &keyword);
int readOption();
void showResult(char result[]);

int main(int argc, char* argv[]) {
	char matrix[SIZE][SIZE];
	char* result = NULL;
	char* temporary;
	string wordToEncrypt;
	string key;
	string keyword;
	int size, rank, number, numberReminder, textLength;
	int option = 0;	
	double start, end;

	VigenereCipher* cipher = new VigenereCipher();
	MatrixHelper* matrixHelper = new MatrixHelper();
	AbstractTimer* timer = NULL;

	// start the communicator
	MPI_Init(NULL, NULL);

	// Get the process' id and MPI_COMM_WORLD represents the default communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Get the size of the communicator
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	// initialize the matrix for each process
	matrixHelper->initializeMatrix(matrix);
	
	if (rank == 0) {
		timer = new MPITimer();
		cout << "~~~~~~~~~~~~~~~~~ Program settings ~~~~~~~~~~~~~~~~~" << endl;
		cout << "Number of processes: " << size << endl;
		cout << "Tabula recta:" << endl ;
		matrixHelper->printMatrix(matrix);
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

		// read the text and the keyword
		readWord(wordToEncrypt);
		readKeyword(keyword);

		// compute the character groups (the last process will receive the remaining characters
		textLength = wordToEncrypt.length();
		int quotient = textLength / size;
		int reminder = textLength % size;
		
		// construct the key based on the processed keyword
		timer->start();
		start = MPI_Wtime();
		key = cipher->constructKey(textLength, keyword);
		cout << "The generated key is: " << key << endl;
		timer->stop();
		end = MPI_Wtime();
		cout << "The creation of the key took " << timer->getDuration() << "ms" << endl;

		// Send the number of elements, the subarray from the word and the corresponding subarray of key
		if (size > 1) {
			timer->start();
			for (int processRank = 1; processRank < size; processRank++) {
				if (processRank != size - 1) {
					MPI_Send(&quotient, 1, MPI_INT, processRank, 0, MPI_COMM_WORLD);
					string wordComponent = wordToEncrypt.substr(processRank * quotient, quotient);
					string keyComponent = key.substr(processRank * quotient, quotient);
					MPI_Send(wordComponent.c_str(), quotient, MPI_CHAR, processRank, 0, MPI_COMM_WORLD);
					MPI_Send(keyComponent.c_str(), quotient, MPI_CHAR, processRank, 0, MPI_COMM_WORLD);
				}
				else {
					int number = quotient + reminder;
					MPI_Send(&number, 1, MPI_INT, processRank, 0, MPI_COMM_WORLD);
					string wordComponent = wordToEncrypt.substr(processRank * quotient, quotient + reminder);
					string keyComponent = key.substr(processRank * quotient, quotient + reminder);
					MPI_Send(wordComponent.c_str(), quotient + reminder, MPI_CHAR, processRank, 0, MPI_COMM_WORLD);
					MPI_Send(keyComponent.c_str(), quotient + reminder, MPI_CHAR, processRank, 0, MPI_COMM_WORLD);
				}
			}
			wordToEncrypt = wordToEncrypt.substr(0, quotient);
			key = key.substr(0, quotient);
			number = quotient;
			numberReminder = quotient + reminder;
			timer->stop();
			cout << "Splitting and sending the word and the key: " << timer->getDuration() << "ms" << endl;
		}
		else {
			number = textLength;
		}
		
		option = readOption();
		timer->start();
	}
	MPI_Bcast(&option, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank != 0) {
		MPI_Status status;
		// Receive the information
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		char* temp = new char[number + 1];
		MPI_Recv(temp, number, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
		wordToEncrypt.assign(temp, number);
		MPI_Recv(temp, number, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
		key.assign(temp, number);
		delete[] temp;
	}
	temporary = new char[number + 1];
	
	if (option == 1) {
		cipher->encrypt(temporary, number, matrix, key, wordToEncrypt);
	}
	else {
		cipher->decrypt(temporary, number, matrix, key, wordToEncrypt);
	}
	delete cipher;

	// Send the results
	if (rank != 0) {
		MPI_Send(temporary, number, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}

	// Receive the results
	if (rank == 0) {
		result = new char[textLength + 1];
		// the master will insert its letters
		for (int iterator = 0; iterator < strlen(temporary); iterator++) {
			result[iterator] = temporary[iterator];
		}
		// insert the letters processed by the other processes
		for (int iterator = 1; iterator < size; iterator++) {
			MPI_Status status;
			if (iterator != size - 1) {
				MPI_Recv(result + iterator * number, number, MPI_CHAR, iterator, 0, MPI_COMM_WORLD, &status);
			}
			else {
				MPI_Recv(result + iterator * number, numberReminder, MPI_CHAR, iterator, 0, MPI_COMM_WORLD, &status);
			}
		}
		result[textLength] = '\0';

		// stop the benchmarking
		timer->stop();
		cout << "Total duration: " << timer->getDuration() << "ms" << endl;
		
		// show the results
		showResult(result);
		delete timer;
	}

	//// cleanup
	delete matrixHelper;
	delete[] result;
	delete[] temporary;
	
	MPI_Finalize();
}

/*
	This is a method used to show the result.
*/
void showResult(char result[]) {
	string option;
	string temp;
	cout << "Do you want to write the result into the output.txt file? (y|n)" << endl;
	cin >> option;
	if (option == "y") {
		// write the result in file
		ofstream outputFile("output.txt");
		if (outputFile.is_open()) {
			outputFile << result;
		}
		outputFile.close();
	}
	else {
		// wite the result in console
		cout << "The result is: ";
		for (int iterator = 0; iterator < strlen(result); iterator++) {
			cout << result[iterator];
		}
		cout << endl;
	}
	
}

/*
* Read the text that will be encrypted/decrypted and process it (a word will be generated)
*/
void readWord(string &wordToEncrypt) {
	string option;
	string temp;

	// read the text that will be processed
	cout << "Do you want to get the text from the input.txt file? (y|n)" << endl;
	cin >> option;
	if (option == "y") {
		// read the text from a file
		ifstream inputFile("input.txt");
		string tempText;
		while (inputFile.good()) { // always check whether the file is open
			inputFile >> tempText; // pipe file's content into stream
			wordToEncrypt.append(tempText);
		}
		inputFile.close();
	}
	else {
		// read the text from the console
		getline(cin, temp);
		cout << "Enter the text you want to encrypt: ";
		getline(cin, wordToEncrypt);
	}

	// process text
	regex pattern(PREPROCESSING_PATTERN);
	wordToEncrypt = regex_replace(wordToEncrypt, pattern, "");
	transform(wordToEncrypt.begin(), wordToEncrypt.end(), wordToEncrypt.begin(), ::toupper);

	cout << "Processed text: " << wordToEncrypt << endl;
}

/*
* Read the keyword used for the encryption/decryption
*/
void readKeyword(string &keyword) {
	// read the keyword
	cout << "Enter the keyword: ";
	cin >> keyword;

	// process the keyword by removing the invalid characters (spaces, punctuation or numbers)
	regex pattern(PREPROCESSING_PATTERN);
	keyword = regex_replace(keyword, pattern, "");

	cout << "Processed keyword: " << keyword << endl;
}

/*
* Read the user's choice (to use the encryption algorithm or the decryption one)
*/
int readOption() {
	int option;
	bool validOption = false;

	while (!validOption) {
		cout << "Choose the desired operation that will be performed:" << endl;
		cout << "1. Vegenere cipher;" << endl;
		cout << "2. Vegenere decipher;" << endl;
		cin >> option;

		validOption = option == 1 || option == 2;
		if (!validOption) {
			cout << "Invalid option... Please try again" << endl;
		}
	}
	
	return option;
}