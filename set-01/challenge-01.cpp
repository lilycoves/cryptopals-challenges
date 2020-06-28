
#include "../conversions.hpp"
#include "../base64.hpp"

int main()
{
    // challenge 1: convert hex to base64 (https://cryptopals.com/sets/1/challenges/1)
    //
    // input: 49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d
    // expected output: SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t
    
    std::string input { "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d" };
    std::string expectedOutput { "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t" };
    
    std::string encoded = b64encode( hex2bin(input) );
    
    std::cout << "Expected output: \n\t" << expectedOutput << std::endl;
    std::cout << "Actual output: \n\t" << encoded << std::endl;
    
    if ( expectedOutput == encoded )
    {
        std::cout << "[SUCCESS] Expected == Actual" << std::endl;
    }
    else
    {
        std::cout << "[ERROR] Expected != Actual" << std::endl;
    }
    
    return 0;
}