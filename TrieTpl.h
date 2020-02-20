#pragma once

#include <array>
#include <queue>
#include <string>

namespace ppittawat::trie {

using func_char_to_uint = unsigned(*)( char );
using func_uint_to_char= char(*)( unsigned );

template <
    size_t children_size
    , func_char_to_uint func_ctoi
    , func_uint_to_char func_itoc
>
struct TrieNodeTpl
{
    using children_type = std::array< TrieNodeTpl *, children_size >;

    ~TrieNodeTpl();

    TrieNodeTpl();
    explicit TrieNodeTpl( TrieNodeTpl * _parent );

    TrieNodeTpl * child( char c );
    const TrieNodeTpl * child( char c ) const;
    TrieNodeTpl * child( unsigned i );
    const TrieNodeTpl * child( unsigned i ) const;

    TrieNodeTpl * parent;
    children_type children;
    /*
     * node with end can form a word.
     * node with end may or may not be a leaf node, eg 'a', 'aa'
     */
    bool end;
};

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc >
class TrieTpl
{
public:
    using node_type = TrieNodeTpl< children_size, func_ctoi, func_itoc >;

    //currently only support const_iterator with forward only.
    /*
     * Note: this iterator MUST NOT be used after the TrieTpl is destroy.
     *       ** because of the dangling pointer with will cause seg fault
     */
    class const_iterator:
        public std::iterator <
            std::input_iterator_tag,    //category
            std::string,                //value_type
            std::string,                //difference_type
            const std::string *,        //pointer
            const std::string & >       //reference
    {
    public:
        explicit const_iterator( const node_type * root );

        const_iterator & operator++();
        const_iterator operator++(int);
        bool operator==( const const_iterator & other ) const;
        bool operator!=( const const_iterator & other ) const;
        reference operator*() const;

    private:
        const node_type * m_root;
        const node_type * m_cur;
        std::string m_val;
    }; // end class const_iterator

    ~TrieTpl();

    TrieTpl();
    TrieTpl( const TrieTpl & other );
    TrieTpl( TrieTpl && other );

    TrieTpl & operator=( TrieTpl && ) = default;
    TrieTpl & operator=( TrieTpl other );

    void insert( const std::string & word );
    bool search( const std::string & word ) const;

    const_iterator begin() const;
    const_iterator end() const;

    const node_type * node() const;

    friend void swap( TrieTpl & lhs, TrieTpl & rhs );

private:
    node_type * m_root;
}; // end TrieTpl

} // end namespace ppittawat::trie

//just to separate implementation
#include "TrieTpl.tcc"

