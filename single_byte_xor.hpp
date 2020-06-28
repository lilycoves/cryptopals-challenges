#include <cmath>

#include "conversions.hpp"

#ifndef SINGLE_BYTE_XOR_HPP
#define SINGLE_BYTE_XOR_HPP

/**
 *  @brief single byte xor encode/decode
 *  
 *  @param [in] data data to be xor'd with key
 *  @param [in] key  byte to be xor'd with input
 *  @return byte array of data[i] ^ key for i=0 to data.size()
 *  
 *  @details n/a
 */
std::vector<uint8_t> singleByteXor( std::vector<uint8_t> data, uint8_t key )
{
    std::vector<uint8_t> output {};
    
    for ( int i = 0; i < data.size(); ++i )
    {
        output.push_back( data[i] ^ key );
    }
    
    return output;
}

/**
 *  @brief calculates the frequency of letters [A-Za-z] and spaces in a string
 *  
 *  @param [in] text input string to calculate letter frequency for
 *  @return normalized frequencies for each letter
 *  
 *  @details return vector is going to contain 27 items, as we are counting
 *      capital and lowercase letters as the same value. so, for example,
 *      'a' and 'A' in a string with both count toward index 0 of the returned vector
 */
std::vector<double> calcLetterFreqs( std::string text )
{
    std::vector<double> freqs(27, 0.0);
    
    // count the letter frequencies in the string. bit of weird-looking math but
    // we dont care about case - 'A' counts the same as 'a', so we need to make
    // sure that we group them in the same category.
    for ( auto c : text )
    {
        if ( c >= 'A' && c <= 'Z' )
        {
            freqs[c - 'A'] += 1;
        }
        else if ( c >= 'a' && c <= 'z' )
        {
            freqs[c - 'a'] += 1;
        }
        else if ( c == ' ' )
        {
            freqs[26] += 1;
        }
    }
    
    // and then normalize them into ratios by dividing the counts by the total
    // length of the string. this normalization is important, otherwise we are just
    // returning letter counts, and that does not necessarily reflect frequency..
    //
    // it would also throw comparisons way off for strings of diff lengths, etc.
    int len = text.size();
    
    for ( int i = 0; i < freqs.size(); ++i )
    {
        freqs[i] = freqs[i] / len;
    }
    
    return freqs;
}

/**
 *  @brief scores a piece of text based on how closely it matches the letter frequency
 *      distribution of the english language.
 *  
 *  @param [in] text string to score
 *  @return overall score of the text. a higher score means a closer resemblance
 *      to the english language frequency table
 *  
 *  @details More details
 */
double scoreText( std::string text )
{
    // score text using the Bhattacharyya Coefficient.
    // 
    // this coefficient can be used to calculate the amount of similarity
    // between two distributions. in our case, its the distributions
    // between the ciphertext and the standard english language letter 
    // frequency
    //
    // we collect the frequency the english language letters, then that of
    // the ciphertext, and then the coefficient is calculated as the sum of the
    // sqrt of the product of all frequencies.
    //
    // the theory behind this is that the ciphertext xord with the correct key
    // will have a frequency distribution closest to english language letter
    // frequency, relative to ciphertext xord with an incorrect key.
    
    static std::vector<double> letterFreqs
    { 
        0.06532, 0.01258, 0.02233, 0.03282, 0.10266, // A-E
        0.01983, 0.01624, 0.04978, 0.05668, 0.00097, // F-J
        0.00560, 0.03317, 0.02026, 0.05712, 0.06159, // K-O
        0.01504, 0.00083, 0.04987, 0.05317, 0.07516, // P-T
        0.02275, 0.00796, 0.01703, 0.01409, 0.01427, // W-Y
        0.00051, 0.10266                             // Z, Space
    };
    
    std::vector<double> textFreqs = calcLetterFreqs( text );
    
    double coefficient = 0.0;
    
    for ( int i = 0; i < letterFreqs.size(); ++i )
    {
        coefficient += std::sqrt( letterFreqs[i] * textFreqs[i] );
    }
    
    return coefficient;
}

/**
 *  @brief brute forces the most likely key for an input that has been xord against
 *      a single byte
 *  
 *  @param [in] input single-byte xor encoded byte array input to brute force
 *  @return most likely key for the encoded byte array. note: right now this
 *      only really takes into consideration byte arrays representative of
 *      english language strings.
 *  
 *  @details this is done by iterating through values 0x00 through 0xff, applying
 *      a single-byte xor with the iterated key, and then scoring the letter
 *      frequency distribution of each key value. we ultimately pick the key that
 *      scores the highest (aka matches most closely to english letter frequency).
 *
 *      this is not perfect, and does not have anything in it to guarantee that
 *      the chosen key is correct, and may not work for inputs that are too short,
 *      but still.
 */
uint8_t bruteForceSingleByteXor( std::vector<uint8_t> input )
{
    double highScore = -1;
    uint8_t key = 0x00;
    
    // we try bytes 0x00 through 0xff, aka every possible value a byte can have
    // because this is a SINGLE-byte xor, remember?
    for ( int i = 0x00; i < 0x100; ++i )
    {
        uint8_t tmpKey = static_cast<uint8_t>( i );

        std::string decoded = bin2ascii( singleByteXor(input, tmpKey), true );
        
        double score = 0.0;
        score = scoreText( decoded );
        
        if ( score > highScore )
        {
            highScore = score;
            key = tmpKey;
        }
    }
    
    return key;
}

#endif