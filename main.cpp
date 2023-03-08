/*
 Alexander Mendoza
 
 */
#include <iostream>
#include <string>
#include <bitset>
#include <random>

std::string DES();
//declaring global to pass through function
std::string kKeys[16];

//turns decimal to binary
//needed for s boxes
std::string decimalToBinary(int dec){
    std::string binary;
    
    while (dec!=0 ){
        binary =(dec % 2 == 0 ? "0":"1") +binary;
        dec = dec/2;
    }
    while(binary.length()<4){
        binary = "0" + binary;
    }
    return binary;
}

int binaryToDecimal(std::string binary){
    int decimal = 0;
    int counter = 0;
    int size = binary.length();
    
    for (int i = size-1; i>=0; i--){
        if (binary[i] == '1'){
            decimal += pow(2,counter);
        }
        counter++;
    }
    return decimal;
}

std::string generateIV(){
    // Create a random number generator using the Mersenne Twister algorithm
    std::mt19937_64 rng(std::random_device{}());
    
    // Generate a 64-bit random number to use as the IV
    uint64_t iv = rng();
    
    // Convert the IV to a binary string
    std::string iv_str = std::bitset<64>(iv).to_string();
    
    // Output the IV as a binary string
    std::cout << iv_str << std::endl;
    
    return iv_str;
}


std::string xorString(std::string x, std::string y){
    std::string result ="";
    int size = y.size();
    //essentially running through a loop of size y that checks 0,1 in string form
    for (int i =0; i<size; i++){
        if (x[i] != y[i]){
            result += "1";
        }
        else {
            result+="0";
        }
    }
    return result;
}

std::string rotate_once(std::string keyStr){
    std::string rotated="";
    
    //index at 1 because of rotation
    for (int i=1; i<28; i++){
        rotated +=keyStr[i];
    }
    //finishing rotation by adding first bit at the end
    rotated += keyStr[0];
    return rotated;
}

std::string rotate_twice(std::string keyStr){
    std::string rotated="";
    
    //run loop twice because of two rotations
    for(int i=0; i<2; i++){
        //index at 1 because of rotation
        for (int j=1; j<28; j++){
            rotated +=keyStr[j];
        }
        //placing first bit at the end
        //storing rotated string as new string to rotate once more
        rotated += keyStr[0];
        keyStr = rotated;
        rotated = "";
    }
    return keyStr;
}




void keys(std::string key){
    //PC1 Table
    int pc1[56]{
        57,49,41,33,25,17,9,
        1,58,50,42,34,26,18,
        10,2,59,51,43,35,27,
        19,11,3,60,52,44,36,
        63,55,47,39,31,23,15,
        7,62,54,46,38,30,22,
        14,6,61,53,45,37,29,
        21,13,5,28,20,12,4
    };
    
    //PC2 Table
    int pc2[48] = {
        14,17,11,24,1,5,
        3,28,15,6,21,10,
        23,19,12,4,26,8,
        16,7,27,20,13,2,
        41,52,31,37,47,55,
        30,40,51,45,33,48,
        44,49,39,56,34,53,
        46,42,50,36,29,32
    };
    
    std::string pKey ="";
    
    //passing pKey to PC1 table
    //64 bit to 56 bit
    
    for (int i =0; i<56; i++){
        pKey += key[pc1[i]-1];
    }
    
    //divide this key into two equal halves C and D
    std::string c = pKey.substr(0,28);
    std::string d = pKey.substr(28,28);
    
    //16 rounds of key generation
    //according to DES round 1,2,9,16 two halves are each rotated left by one bit
    //all other rounds two will be rotated to left
    
    for (int i =0; i<16; i++){
        if (i == 0 ||i==1 || i==8 || i==15){
            //rotate once
            c = rotate_once(c);
            d= rotate_once(d);
        }
        else {
            c = rotate_twice(c);
            d = rotate_twice(d);
        }
        
        std::string combined = c+d;
        std::string kKey ="";
        
        //passing kKey into pc2 table, will be stored in a kKey array of size 16
        //48 bits
        for (int i=0; i<48; i++){
            kKey+= combined[pc2[i]-1];
        }
        kKeys[i]= kKey;
    }
}




std::string DES(std::string plainTxt){
    
    //declaring all tables/matrices
    int INIT_PERM[64]{
        58,50,42,34,26,18,10,2,
        60,52,44,36,28,20,12,4,
        62,54,46,38,30,22,14,6,
        64,56,48,40,32,24,16,8,
        57,49,41,33,25,17,9,1,
        59,51,43,35,27,19,11,3,
        61, 53,45,37,29,21,13,5,
        63,55,47,39,31,23,15,7
    };
    
    int INV_PERM[64]{
        40,8,48,16,56,24,64,32,
        39,7,47,15,55,23,63,31,
        38,6,46,14,54,22,62,30,
        37,5,45,13,53,21,61,29,
        36,4,44,12,52,20,60,28,
        35,3,43,11,51,19,59,27,
        34,2,42,10,50,18,58,26,
        33,1,41,9,49,17,57,25
    };
    
    int EXPANSION[48]{
        32,1,2,3,4,5,
        4,5,6,7,8,9,
        8,9,10,11,12,13,
        12,13,14,15,16,17,
        16,17,18,19,20,21,
        20,21,22,23,24,25,
        24,25,26,27,28,29,
        28,29,30,31,32,1
    };
    
    int PERM_FUNCTION[32]{
        16,7,20,21,29,12,28,17,
        1,15,23,26,5,18,31,10,
        2,8,24,14,32,27,3,9,
        19,13,30,6,22,11,4,25
    };
    
    
    //read as eight 4x16 matrices
    int S_BOXES[8][4][16]{
        {  14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
            0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
            4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
            15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
        },
        {
            15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
            3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
            0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
            13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
        },
        {
            10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
            13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
            13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
            1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
        },
        {
            7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
            13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
            10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
            3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
        },
        {
            2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
            14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
            4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
            11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
        },
        {
            12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
            10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
            9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
            4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
        },
        {
            4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
            13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
            1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
            6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
        },
        {
            13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
            1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
            7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
            2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
        }};
    
    
    //initial permutation
    std::string initPerm = "";
    
    for (int i =0; i<64;i++){
        //-1 for indexing through the array ex: 64 is out of the range
        initPerm += plainTxt[INIT_PERM[i]-1];
    }
    
    //splitting left & right from initial permutation
    std::string left = initPerm.substr(0,32);
    std::string right = initPerm.substr(32,32);
    
    //encrypt 16 times
    for (int i=0; i<16; i++){
        std::string rightExpansion = "";
        //need to expand right side from 32 bits to 48 bits
        for(int i = 0; i<64; i++){
            rightExpansion += right[EXPANSION[i]-1];
        }
        //get the right side and xor with a key from table (f) III
        std::string xored = xorString(kKeys[i],rightExpansion);
        std::string result ="";
        
        //III.3 8 substitution boxes of 6 bits
        for (int i =0; i <8; i++){
            
            //this grabs the first bit and last bit of the xored to get the row from sbox
            std::string rowStr = xored.substr(i*6,1) +xored.substr(i*6 +5,1);
            int row = binaryToDecimal(rowStr);
            
            //this grabs the bits between the first and last bit
            std::string colStr = xored.substr(i*6 + 1,1) + xored.substr(i*6 + 2,1) + xored.substr(i*6 + 3,1) + xored.substr(i*6 + 4,1);
            int col = binaryToDecimal(colStr);
            
            //after getting the row & column, this grabs the number from one of the 4x16 matrix
            //i represents which of the 8 matrices to chose
            int x = S_BOXES[i][row][col];
            result += decimalToBinary(x);
        }
        //creating permutation
        std::string perm = "";
        //running through f function of 32 bits
        for (int i = 0; i<32; i++){
            perm += result[PERM_FUNCTION[i]-1];
        }
        xored= xorString(perm,left);
        left = xored;
        
        if (i<15){
            //as per the algorithm swap left and right every iteration
            std::string temp = right;
            right = xored;
            left = temp;
        }
    }
    //combine left &right before last permutation
    std::string lr = left + right;
    std::string cipherTxt = "";
    //run on the inverse perm table
    for (int i=0; i<64; i++){
        cipherTxt += lr[INV_PERM[i]-1];
    }
    
    return cipherTxt;
}



int main() {
    
    std::string key= "1010101010111011000010010001100000100111001101101100110011011101";
    
    std::string plainTxt = "1010101111001101111001101010101111001101000100110010010100110110";
    
    keys(key);
    std::cout<<"Plain txt: " <<plainTxt<<"\n";
    std::string ct=DES(plainTxt);
    std::cout<<"Cipher txt:" <<ct<<"\n";
    
    
    
    
    
    int i = 15;
    int j = 0;
    
    while (i>j){
        
        //doing the key swap in reverse orer from the encryption
        std::string temp = kKeys[i];
        kKeys[i] = kKeys[j];
        kKeys[j] = temp;
        i--;
        j++;
    }

    //setting plain text to be the encrypted text
    plainTxt = ct;
    std::string decrypted;
    decrypted = DES(plainTxt);
    
    std::cout<<"decrypted txt: "<<decrypted<<std::endl;
    

    return 0;
}
