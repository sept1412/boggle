#pragma once

#include "TrieTpl.h"

namespace ppittawat::trie {

/**
 * A trie for uppercase english characters only
 */

constexpr unsigned EN_ALPHABET_SIZE =  26;
constexpr unsigned EnAlphabetToIndex( char c ) { return ( c - 'A' ); }
constexpr char IndexToEnAlphabet( unsigned i ) { return i + 'A'; }

using Trie = TrieTpl<EN_ALPHABET_SIZE, EnAlphabetToIndex, IndexToEnAlphabet >;

/**
 * @param valid format is that
 *      each line of the file contain 1 word
 *      only 'A' - 'Z' is valid
 *
 * TODO: move this to utils or anything that is more specific to EN letters Trie
 */
Trie loadTrie( const char * file );

void test_trie( const char * file );

} // end namespace ppittawat::trie
