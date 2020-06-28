#include <iostream>

#include "../conversions.hpp"
#include "../single_byte_xor.hpp"

int main()
{
    // challenge 3: single-byte xor (https://cryptopals.com/sets/1/challenges/3)
    //
    // input: 1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736
    //
    // we are to brute force the key for this and output its value. the output will
    // be easily recognizeable as english text
    
    std::string input { "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736" };
    
    uint8_t key = bruteForceSingleByteXor( hex2bin(input) );
    
    std::cout << "Key: " << std::hex << std::showbase << static_cast<int>(key) << std::endl;
    
    std::string decoded = bin2ascii( singleByteXor( hex2bin(input), key), true );
    std::cout << "Decoded output: " << decoded << std::endl;
    
    return 0;
}