// CPP Header Files.

#include <fstream>  
#include <iostream>  
#include <string>  
#include <ctime>
#include <dirent.h>
using namespace std;   

// Utility Header Files.

#include "keyGenerator.h"
#include "byteOperations.h"
#include "encrypt.h"
#include "chunking.h"
#include "reGrouping.h"



// Typedef Utilities
typedef bitset<8> byte;
typedef bitset<32> word;  


//Round constant, used in key expansion. (AES-128 only takes 10 rounds)  
word Rcon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,   
                 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000}; 


 
word Word(byte& k1, byte& k2, byte& k3, byte& k4)  
{  
    word result(0x00000000);  
    word temp;  
    temp = k1.to_ulong();  // K1  
    temp <<= 24;  
    result |= temp;  
    temp = k2.to_ulong();  // K2  
    temp <<= 16;  
    result |= temp;  
    temp = k3.to_ulong();  // K3  
    temp <<= 8;  
    result |= temp;  
    temp = k4.to_ulong();  // K4  
    result |= temp;  
    return result;  
}  
  
/** 
 *  Cyclic left shift by byte 
 *  That is to say, [a0, a1, a2, a3] becomes [a1, a2, a3, a0] 
 */  
word RotWord(word& rw)  
{  
    word high = rw << 8;  
    word low = rw >> 24;  
    return high | low;  
}  
  
/** 
 *  S-box transformation for each byte in input word 
 */  
word SubWord(const word& sw)  
{  
    word temp;  
    for(int i=0; i<32; i+=8)  
    {  
        int row = sw[i+7]*8 + sw[i+6]*4 + sw[i+5]*2 + sw[i+4];  
        int col = sw[i+3]*8 + sw[i+2]*4 + sw[i+1]*2 + sw[i];  
        byte val = S_Box[row][col];  
        for(int j=0; j<8; ++j)  
            temp[i+j] = val[j];  
    }  
    return temp;  
}  

void KeyExpansion(byte key[4*Nk], word w[4*(Nr+1)])  
{  
    word temp;  
    int i = 0;  
    //The first four of w [] are input key s  
    while(i < Nk)   
    {  
        w[i] = Word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3]);  
        ++i;  
    }  
  
    i = Nk;  
  
    while(i < 4*(Nr+1))  
    {  
        temp = w[i-1]; //Record the previous word  
        if(i % Nk == 0)  
            w[i] = w[i-Nk] ^ SubWord(RotWord(temp)) ^ Rcon[i/Nk-1];  
        else   
            w[i] = w[i-Nk] ^ temp;  
        ++i;  
    }  
}




int main()
{

    // // INTRODUCTION.
    // cout<<"WELCOME TO PROJECT HORCRUX, YOUR PRIVACY IS OUR PRIORITY";
    // cout<<endl;


    // cout<<"Please enter your choice-"<<endl;
    // cout<<"Press 'E' for Encrypting your file."<<endl;
    // cout<<"Press 'D' for Decrypting your file."<<endl;

    srand(time(NULL)); 


    cout<<"Generating secret key kindly store it for future reference...."<<endl;
    
    string keyStr = printRandomString(); 
    
    cout << "The key is:"<<keyStr<<endl;
    
    byte key[16];   
    charToByte(key, keyStr.c_str()); 

    // Key Expansion
    word w[4*(Nr+1)];  
    KeyExpansion(key, w); 

    cout<<endl; 


    bitset<128> data;  

    byte plain[16]; 

    // Ask for location of the file.

    cout<<"Enter the Path of the File you want to encrypt."<<endl;

    string path;
    getline(cin,path);


    cout<<"Encrypting the File."<<endl;

    // Encrypt

    ifstream in;  
    ofstream out;  

    in.open(path.c_str(),ios::in); 
    out.open("cipher.txt", ios::binary);  

    while(in.read((char*)&data, sizeof(data)))  
    {  
        divideToByte(plain, data);  
        encrypt(plain, w);  
        data = mergeByte(plain);  
        out.write((char*)&data, sizeof(data));  
        data.reset();  //Set 0  
    }  
    in.close();  
    out.close();  

    cout<<"File Encrypted Successfully."<<endl;


    // after the path of file is being taken a new encrypted called --> cypher text
    //  is being generated and we divide this cipher text into chunks.


    ifstream in_file("cipher.txt", ios::binary); 
    in_file.seekg(0, ios::end);
    int file_size = in_file.tellg();
    cout<<"Size of the file is"<<" "<< file_size<<" "<<"bytes";

    cout<<endl;

    cout<<"Enter path where you want to store the chunks:";
	string chunkPath;
    getline(cin,chunkPath);

    int chunkSize;
	cout<<endl;
	cout<<"Enter how many chunks you want to create : "<<endl;
	cin>>chunkSize;

	int chunkByte = file_size/chunkSize;


    // Code for dividing into chunks.

    cout<<"Creating the File Chunks."<<endl;

    chunkFile("cipher.txt", "chunk", chunkByte+1,chunkPath);

    string decryptKey;

    cout<<"Enter Key for Decryption:";
    cin>>decryptKey;

    string newpath = "D:\\Cubbit Assesment\\The-Horcrux-Project\\SourceFiles\\";
    string oldpath = chunkPath+"\\";
    

    if(decryptKey==keyStr)
    {
        cout<<"The Key is Right."<<endl;
        cout<<"Regrouping the File."<<endl;

        //before regrouping hame chunkFiles k ander ka material bahar sourcefolder maio dalna hai

        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (oldpath.c_str())) != NULL) 
        {
            /* print all the files and directories within directory */
            while ((ent = readdir (dir)) != NULL) 
            {
                printf ("%s\n", ent->d_name);
                rename((oldpath+ent->d_name).c_str(),(newpath+ent->d_name).c_str());
            }
            closedir (dir);
        } 
        else 
        {
            /* could not open directory */
            perror ("");
            return EXIT_FAILURE;
        }


        joinFile("chunk", "regrouped.txt");


        in.open("regrouped.txt", ios::binary);  
        out.open("DecryptedText.txt", ios::binary);  
        while(in.read((char*)&data, sizeof(data)))  
        {  
            divideToByte(plain, data);  
            decrypt(plain, w);  
            data = mergeByte(plain);  
            out.write((char*)&data, sizeof(data));  
            data.reset();  //Set 0  
        }  
        in.close();  
        out.close();  

        cout<<"File has been Decrypted Successfully."<<endl;

       
    }
    else
    {
        cout<<"Sorry! The Key is not valid.";
    }

    return 0;
}