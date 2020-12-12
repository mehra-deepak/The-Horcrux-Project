 #include<iostream>
 #include<fstream>
 using namespace std;


// Chunks a file by breaking it up into chunks of "chunkSize" bytes.
void chunkFile(char *fullFilePath, char *chunkName, unsigned long chunkSize) {
	ifstream fileStream;
	fileStream.open(fullFilePath, ios::in | ios::binary);

	// File open a success
	if (fileStream.is_open()) {
		ofstream output;
		int counter = 1;

		string fullChunkName;

		// Create a buffer to hold each chunk
		char *buffer = new char[chunkSize];

		// Keep reading until end of file
		while (!fileStream.eof()) {

			// Build the chunk file name. Usually drive:\\chunkName.ext.N
			// N represents the Nth chunk
			fullChunkName.clear();
			fullChunkName.append(chunkName);
			fullChunkName.append(".");

			// Convert counter integer into string and append to name.
			char intBuf[10];
			itoa(counter,intBuf,10);
			fullChunkName.append(intBuf);

			// Open new chunk file name for output
			output.open(fullChunkName.c_str(),ios::out | ios::trunc | ios::binary);

			// If chunk file opened successfully, read from input and 
			// write to output chunk. Then close.
			if (output.is_open()) { 
				fileStream.read(buffer,chunkSize);
				// gcount() returns number of bytes read from stream.
				output.write(buffer,fileStream.gcount());
				output.close();

				counter++;
			}
		}

		// Cleanup buffer
		delete(buffer);

		// Close input file stream.
		fileStream.close();
		cout << "Chunking complete! " << counter - 1 << " files created." << endl;
	}
	else { cout << "Error opening file!" << endl; }
}