#include <iostream>

#include "../conversions.hpp"
#include "../fixed_xor.hpp"

int main()
{
    // challenge 2: fixed xor (https://cryptopals.com/sets/1/challenges/2)
    //
    // input: 1c0111001f010100061a024b53535009181c
    // key: 686974207468652062756c6c277320657965
    // expected output: 746865206b696420646f6e277420706c6179
    
    std::string input { "1c0111001f010100061a024b53535009181c" };
    std::string key { "686974207468652062756c6c277320657965" };
    
    std::string expectedOutput { "746865206b696420646f6e277420706c6179" };
    
    std::vector<uint8_t> cipher = fixedXor( hex2bin(input), hex2bin(key) );
    
    std::string output = bin2hex( cipher );
    
    std::cout << "Expected output: \n\t" << expectedOutput << std::endl;
    std::cout << "Actual output: \n\t" << output << std::endl;
    
    if ( expectedOutput == output )
    {
        std::cout << "[SUCCESS] Expected == Actual" << std::endl;
    }
    else
    {
        std::cout << "[ERROR] Expected != Actual" << std::endl;
    }
}