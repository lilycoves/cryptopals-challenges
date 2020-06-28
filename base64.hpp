#include <string>
#include <cstdint>
#include <iostream>

#include "conversions.hpp"

#ifndef BASE64_HPP
#define BASE64_HPP

/**
 *  @brief converts a byte array into its base64 encoding
 *  
 *  @param [in] data the data to b64 encode
 *  @return b64 encoding of input
 *  
 *  @details n/a
 */
std::string b64encode( std::vector<uint8_t> data )
{
    static char const charset[] { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };
    
    std::string encoding { "" };
    
    // break data into 3-byte segments
    int numBlocks = data.size() / 3;
    int padding   = 3 - (data.size() % 3);
    
    // we operate on at least one block - padding it if necessary
    if ( numBlocks == 0 )
        { numBlocks = 1; }
    
    for ( int i = 0; i < numBlocks; ++i )
    {
        // position in the data array, based on the number of blocks
        // that weve processed so far
        int index = (i * 3);
        
        int block = 0;
        
        // build the block out of either data bytes or padding
        block  = ((index   >= data.size()) ? 0x0 : data[index]) << 16;
        block += ((index+1 >= data.size()) ? 0x0 : data[index+1]) << 8;
        block += ((index+2 >= data.size()) ? 0x0 : data[index+2]);
        
        // now that the blocks have been built, break it up into four groups
        // of six bits each
        int group1 = (block >> 18) & 0x00003F;
        int group2 = (block >> 12) & 0x00003F;
        int group3 = (block >> 6)  & 0x00003F;
        int group4 = block & 0x00003F;
        
        // add them into our b64 encoded string - we will handle padding later
        encoding += charset[group1];
        encoding += charset[group2];
        encoding += charset[group3];
        encoding += charset[group4];
    }
    
    // after everythings processed, we need to figure out if the last char or
    // two is padding.
    //
    // yes, this is a shitty way to do it. i know. it looks ugly and i am
    // well aware.
    switch( padding )
    {
        case 1:
            encoding.pop_back();
            encoding += "=";
            break;
        case 2:
            encoding.pop_back();
            encoding.pop_back();
            encoding += "==";
            break;
        default:
            break;
    }
    
    return encoding;
}

/**
 *  @brief converts a b64 encoded string into its respective byte array
 *  
 *  @param [in] b64 encoded string to decode
 *  @return byte array of data 
 *  
 *  @details n/a
 */
std::vector<uint8_t> b64decode( std::string input )
{
    if ( input.size() < 4 )
    {
        throw std::runtime_error( "b64decode(): Input must be at least four characters!" );
    }
    
    if ( (input.size() % 4) != 0 )
    { 
        throw std::runtime_error( "b64decode(): Input must be increment of 4 chars" ); 
    }
    
    static std::string const charset { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=" };
    
    std::vector<uint8_t> decoding {};
    
    int numBlocks = input.size() / 4;
    
    for ( int i = 0; i < numBlocks; ++i )
    {
        int index = (i * 4);
        
        // break it up into groups of four characters, as a proper b64 encoding will
        // always consist of a multiple of 4 characters.
        int group1 = charset.find( input[index] );
        int group2 = charset.find( input[index+1] );
        int group3 = (input[index+2] == '=') ? 0x00 : charset.find( input[index+2] );
        int group4 = (input[index+3] == '=') ? 0x00 : charset.find( input[index+3] );
        
        // convert the values at these four characters back into a single block of data
        // which is basically reverse of encoding process
        int block = 0;
        block += (group1 << 18);
        block += (group2 << 12);
        block += (group3 << 6);
        block += group4;
        
        // from there, separate it at byte-boundaries (8 bits) and drop those in.
        // conversion rate of 4 b64 encoded chars -> 3 bytes.
        decoding.push_back( static_cast<uint8_t>((block & 0xFF0000) >> 16) );
        if ( input[i+2] != '=' ) { decoding.push_back( static_cast<uint8_t>((block & 0x00FF00) >> 8) ); }
        if ( input[i+3] != '=' ) { decoding.push_back( static_cast<uint8_t>(block & 0x0000FF) ); }
    }
    
    return decoding;
}

#endif