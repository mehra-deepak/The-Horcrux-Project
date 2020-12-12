#include<iostream>
#include <bitset> 
#include "operations.h"

using namespace std;

// Typedef Utilities
typedef bitset<8> byte;  
typedef bitset<32> word; 

const int Nr = 10;  //AES-128 requires 10 rounds of encryption  
const int Nk = 4;   //Nk Represents the number of word s that are input keys 





void encrypt(byte in[4*4], word w[4*(Nr+1)])  
{  
    word key[4];  
    for(int i=0; i<4; ++i)  
        key[i] = w[i];  
    AddRoundKey(in, key);  
  
    for(int round=1; round<Nr; ++round)  
    {  
        SubBytes(in);  
        ShiftRows(in);  
        MixColumns(in);  
        for(int i=0; i<4; ++i)  
            key[i] = w[4*round+i];  
        AddRoundKey(in, key);  
    }  
  
    SubBytes(in);  
    ShiftRows(in);  
    for(int i=0; i<4; ++i)  
        key[i] = w[4*Nr+i];  
    AddRoundKey(in, key);  
}  

 
void decrypt(byte in[4*4], word w[4*(Nr+1)])  
{  
    word key[4];  
    for(int i=0; i<4; ++i)  
        key[i] = w[4*Nr+i];  
    AddRoundKey(in, key);  
  
    for(int round=Nr-1; round>0; --round)  
    {  
        InvShiftRows(in);  
        InvSubBytes(in);  
        for(int i=0; i<4; ++i)  
            key[i] = w[4*round+i];  
        AddRoundKey(in, key);  
        InvMixColumns(in);  
    }  
  
    InvShiftRows(in);  
    InvSubBytes(in);  
    for(int i=0; i<4; ++i)  
        key[i] = w[i];  
    AddRoundKey(in, key);  
}