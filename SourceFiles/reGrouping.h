// Finds chunks by "chunkName" and creates file specified in fileOutput


#include<iostream>
#include<fstream>
using namespace std;

// Simply gets the file size of file.
int getFileSize(ifstream *file) {
	file->seekg(0,ios::end);
	int filesize = file->tellg();
	file->seekg(ios::beg);
	return filesize;
}

void joinFile(char *chunkName, char *fileOutput) {
	string fileName;

	// Create our output file
	ofstream outputfile;
	outputfile.open(fileOutput, ios::out | ios::binary);

	// If successful, loop through chunks matching chunkName
	if (outputfile.is_open()) {
		bool filefound = true;
		int counter = 1;
		int fileSize = 0;

		while (filefound) {
			filefound = false;

			// Build the filename
			fileName.clear();
			fileName.append(chunkName);
			fileName.append(".");

			char intBuf[10];
			_itoa(counter,intBuf,10);
			fileName.append(intBuf);

			// Open chunk to read
			ifstream fileInput;
			fileInput.open(fileName.c_str(), ios::in | ios::binary);

			// If chunk opened successfully, read it and write it to 
			// output file.
			if (fileInput.is_open()) {
				filefound = true;
				fileSize = getFileSize(&fileInput);
				char *inputBuffer = new char[fileSize];

				fileInput.read(inputBuffer,fileSize);
				outputfile.write(inputBuffer,fileSize);
				delete(inputBuffer);

				fileInput.close();
			}
			counter++;
		}

		// Close output file.
		outputfile.close();

		cout << "File assembly complete!" << endl;
	}
	else { cout << "Error: Unable to open file for output." << endl; }

}
