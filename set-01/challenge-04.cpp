#include <fstream>
#include <iostream>

#include "../single_byte_xor.hpp"

int main()
{
    // challenge 4: detect single-byte xor (https://cryptopals.com/sets/1/challenges/4)
    //
    // input: lines from a file "data4.txt".
    //
    // challenge text: One of the 60-character strings in this file has been encrypted by single-character XOR. Find it.
    
    std::ifstream file( "../data4.txt" );
    
    std::string line;
    
    if ( file.is_open() )
    {
        double highScore = -1.0;
        
        std::string encryptedString { "" };
        uint8_t key = 0x00;
        
        // we want to pull each line in the file and brute force the most likely
        // single-byte xor key for it. once we get the most likely key, we decrypt
        // the data with it and then score the plaintext. the plaintext with the
        // highest score is our winner.
        while ( std::getline( file, line ) )
        {
            std::vector<uint8_t> byteArray = hex2bin(line);
            
            uint8_t tmpKey = bruteForceSingleByteXor( byteArray );
            
            std::string decoded = bin2ascii( singleByteXor(byteArray, tmpKey), true );
            
            double score = scoreText( decoded );
            
            if ( score > highScore )
            {
                highScore = score;
                encryptedString = line;
                key = tmpKey;
                
            }
        }
        
        std::string decryptedString = bin2ascii( singleByteXor(hex2bin(encryptedString), key), true );
        
        std::cout << "Most likely encrypted string: \n\t" << encryptedString << std::endl;
        std::cout << "Most likely key for encrypted string: " << std::hex << std::showbase << key << std::endl;
        std::cout << "Decrypted string: \n\t" << decryptedString << std::endl;
    }
    else
    {
        throw std::runtime_error( "Unable to open file" );
    }
    
    return 0;
}