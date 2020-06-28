#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#ifndef CONVERSIONS_HPP
#define CONVERSIONS_HPP

/**
 *  @brief Converts a hex char (0-f) to its numerical value. Case-insensitive
 *  
 *  @param [in] c `char` with a value of [0-9a-fA-F]
 *  @return numerical representation of hex character
 *  
 *  @details throws std::runtime_error on bad input
 */
uint8_t decodeHexChar( char c )
{
    if ( c >= '0' && c <= '9' )
    {
        return (c - '0');
    }
    else if ( c >= 'a' && c <= 'f' )
    {
        // add 10 to adjust for the fact that a=10, b=11, etc.
        return (c - 'a') + 10;
    }
    else if ( c >= 'A' && c <= 'F' )
    {
        // add 10 to adjust for the fact that a=10, b=11, etc.
        return (c - 'A') + 10;
    }
    else
    {
        throw std::runtime_error( "decodeHexChar(): Invalid char: " + static_cast<int>(c) );
    }
}

/**
 *  @brief Converts hex string (case-insensitive) to byte array
 *  
 *  @param [in] hexString case-inensitive hex string
 *  @return byte array (std::vector<uint8_t>)
 *  
 *  @details n/a
 */
std::vector<uint8_t> hex2bin( std::string const hexString )
{
    // only handle cases where hexString.length()%2==0 right now. 
    // in theory, handle other lengths by prepending 0 to the beginning
    // of the hexstring (or similar).
    //
    // TODO: figure out if i wanna fix it lol
    
    if ( (hexString.length() % 2) != 0 )
    {
        throw std::runtime_error( "hex2bin(): Invalid hexstring length" );
    }
    
    std::vector<uint8_t> data {};
    
    for ( int i = 0; i < hexString.length()/2; ++i )
    {
        int offset = i * 2;
        
        uint8_t hexByte = decodeHexChar( hexString[offset] ) << 4;
        hexByte += decodeHexChar( hexString[offset+1] );
        
        data.push_back( hexByte );
    }
    
    return data;
}

/**
 *  @brief Encodes a nibble. nybble? dunno. either way.
 *  
 *  @param [in] nibble uint8_t ranging from 0x00 - 0x0f
 *  @return hex char for nibble
 *  
 *  @details n/a
 */
char encodeNibble( uint8_t nibble )
{
    if ( nibble > 0x0f )
    {
        throw std::runtime_error( "encodeNibble(): Input must be between 0x00 and 0x0f" );
    }
    
    if ( nibble <= 0x09 )
    {
        return nibble + '0';
    }
    else
    {
        // offsetting by 10 because a=10, b=11, etc.
        return nibble - 10 + 'a';
    }
}

/**
 *  @brief Converts a byte to hex string
 *  
 *  @param [in] byte byte to convert
 *  @return hex string representation of input
 *  
 *  @details n/a
 */
std::string byte2hex( uint8_t byte )
{
    std::string hexString { "" };
    
    hexString += encodeNibble( (byte >> 4) & 0x0F );
    hexString += encodeNibble( byte & 0x0F );
    
    return hexString;
}

/**
 *  @brief Converts a byte array to a hex string
 *  
 *  @param [in] data Description for data
 *  @return hex string representation of byte array
 *  
 *  @details n/a
 */
std::string bin2hex( std::vector<uint8_t> const data )
{
    std::string hexString { "" };
    
    for ( int i = 0; i < data.size(); ++i )
    {
        hexString += encodeNibble( (data[i] >> 4) & 0x0F );
        hexString += encodeNibble( data[i] & 0x0F );
    }
    
    return hexString;
}

/**
 *  @brief Converts byte array to ASCII string
 *  
 *  @param [in] data byte array to convert
 *  @param [in] safe if true, replace non-printable byte values with a printable ascii char
 *  @return Return description
 *  
 *  @details n/a
 */
std::string bin2ascii( std::vector<uint8_t> const data, bool safe )
{
    std::string output {};
    
    for ( int i = 0; i < data.size(); ++i )
    {
        if ( data[i] < ' ' || data[i] > '~' )
        {
            if ( safe )
            {
                output += "¤";
            }
            else
            {
                output += data[i];
            }
        }
        else
        {
            output += static_cast<char>( data[i] );
        }
    }
    
    return output;
}

/**
 *  @brief Converts ASCII string to byte array
 *  
 *  @param [in] input ascii string to convert
 *  @return byte array representation of input
 *  
 *  @details n/a
 */
std::vector<uint8_t> ascii2bin( std::string const input )
{
    std::vector<uint8_t> output {};
    
    for ( auto c : input )
    {
        output.push_back( static_cast<uint8_t>(c) );
    }
    
    return output;
}

/**
 *  @brief Converts ASCII string to hex string
 *  
 *  @param [in] input ascii string to convert
 *  @return hex string representation of input
 *  
 *  @details convenience function. sue me
 */
std::string ascii2hex( std::string const input )
{
    std::vector<uint8_t> data = ascii2bin( input );
    return bin2hex( data );
}

/**
 *  @brief Converts a hex string to its ASCII string representation
 *  
 *  @param [in] hexstring hex string to convert
 *  @param [in] safe      if true, replace non-printable byte values w/printable ascii char
 *  @return ascii string representation of byte array
 *  
 *  @details another convenience function. sue me.
 */
std::string hex2ascii( std::string const hexstring, bool safe )
{
    return bin2ascii( hex2bin(hexstring), safe );
}

#endif