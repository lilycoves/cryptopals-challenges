#include "conversions.hpp"

#ifndef FIXED_XOR_HPP
#define FIXED_XOR_HPP

/**
 *  @brief xors an input against a key of the same length. think like an OTP
 *  
 *  @param [in] inp plaintext
 *  @param [in] key key to xor against
 *  @return byte array of inp ^ key
 *  
 *  @details plaintext input must be the same size as the key
 */
std::vector<uint8_t> fixedXor( std::vector<uint8_t> inp, std::vector<uint8_t> key )
{
    if ( inp.size() != key.size() )
    {
        throw std::runtime_error( "fixedXor(): Input and key size must match!" );
    }
    
    std::vector<uint8_t> cipher {};
    
    for ( int i = 0; i < inp.size(); ++i )
    {
        cipher.push_back( inp[i] ^ key[i] );
    }
    
    return cipher;
}

#endif
