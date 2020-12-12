#include<iostream>
#include <bitset> 
using namespace std;

// Typedef Utilities
typedef bitset<8> byte;  


/** 
 *  Divide consecutive 128 bits into 16 groups and store them in a byte array 
 */  
void divideToByte(byte out[16], bitset<128>& data)  
{  
    bitset<128> temp;  
    for(int i=0; i<16; ++i)  
    {  
        temp = (data << 8*i) >> 120;  
        out[i] = temp.to_ulong();  
    }  
}  

/** 
 *  Merge 16 byte s into 128 consecutive bits 
 */  
bitset<128> mergeByte(byte in[16])  
{  
    bitset<128> res;  
    res.reset();  //Set 0  
    bitset<128> temp;  
    for(int i=0; i<16; ++i)  
    {  
        temp = in[i].to_ulong();  
        temp <<= 8*(15-i);  
        res |= temp;  
    }  
    return res;  
}  

void charToByte(byte out[16], const char s[16])  
{   
    for(int i=0; i<16; ++i)    
        for(int j=0; j<8; ++j)    
            out[i][j]= ((s[i]>>j) & 1);    
}  