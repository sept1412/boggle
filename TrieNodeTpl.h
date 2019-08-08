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
    using this_type = TrieNodeTpl<children_size, func_ctoi, func_itoc>;
    using children_type = std::array< this_type *, children_size >;

    ~TrieNodeTpl()
    {
        for( auto it = children.begin(); it != children.end(); ++it )
        {
            if( nullptr != (*it) )
            {
                delete (*it);
            }
        }
    }

    TrieNodeTpl():
        parent( nullptr ),
        children(),
        end( false )
    {}

    TrieNodeTpl( TrieNodeTpl * _parent ):
        parent( _parent ),
        children(),
        end( false )
    {}

    TrieNodeTpl * child( char c )
    {
        return children[ func_ctoi( c ) ];
    }

    const TrieNodeTpl * child( char c ) const
    {
        return children[ func_ctoi( c ) ];
    }

    TrieNodeTpl * child( unsigned i )
    {
        return children[ i ];
    }

    const TrieNodeTpl * child( unsigned i ) const
    {
        return children[ i ];
    }

    this_type * parent;
    children_type children;
    /*
     * node with end can form a word.
     * node with end may or may not be a leaf node, eg 'a', 'aa'
     */
    bool end;
}; // end TrieNodeTpl

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc >
class TrieTpl
{
public:
    using node_type = TrieNodeTpl< children_size, func_ctoi, func_itoc >;

    //only give const_iterator
    class const_iterator:
        public std::iterator <
            std::input_iterator_tag,    //category
            std::string,                //value_type
            std::string,                //difference_type
            const std::string *,        //pointer
            const std::string & >       //reference
    {
    public:
        //break at the first word
        explicit const_iterator( const node_type * root ):
            m_root( root ),
            m_cur( root ),
            m_val()
        {
            if( nullptr == m_root )
            {
                return;
            }

            for( unsigned i = 0; i < children_size; )
            {
                if( ! m_cur->children[i] )
                {
                    ++i;
                    continue;
                }

                m_val.push_back( func_itoc( i ) );
                m_cur = m_cur->children[i];
                if( m_cur->end )
                {
                    //found
                    break;
                }
                else
                {
                    //reset the loop
                    i = 0;
                }
            }
        }

        const_iterator & operator++()
        {
            if( ! m_root )
            {
                return *this;
            }

            unsigned i = 0;
            while( nullptr != m_cur )
            {
                for( ; i < children_size; )
                {
                    if( ! m_cur->children[i] )
                    {
                        ++i;
                        continue;
                    }

                    m_val.push_back( func_itoc( i ) );
                    m_cur = m_cur->children[i];
                    if( m_cur->end )
                    {
                        return *this;
                    }
                    else
                    {
                        i = 0;
                    }
                }

                //search root and not found
                if( m_val.empty() )
                {
                    break;
                }

                i = func_ctoi( m_val.back() ) + 1;
                m_cur = m_cur->parent;
                m_val.pop_back();
            }

            //end
            m_root = nullptr;
            m_cur = nullptr;
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator retval = *this;
            ++(*this);
            return retval;
        }

        bool operator==( const const_iterator & other ) const
        {
            return (
                ( m_root == other.m_root )
                && ( m_cur == other.m_cur )
            );
        }

        bool operator!=( const const_iterator & other ) const
        {
            return ! ( *this == other );
        }

        reference operator*() const { return m_val; }

    private:
        const node_type * m_root;
        const node_type * m_cur;
        std::string m_val;
    };

    ~TrieTpl()
    {
        if( m_root )
        {
            delete m_root;
        }
    }

    //simply steal the resource
    TrieTpl( TrieTpl && other ):
        m_root( other.m_root )
    {
        other.m_root = nullptr;
    }

    TrieTpl & operator=( TrieTpl && ) = default;

    TrieTpl():
        m_root( new node_type )
    {
    }

    //need a deep copy here.
    TrieTpl( const TrieTpl & other ):
        m_root( new node_type )
    {
        //BFS Traversal
        std::queue< std::pair<const node_type *, node_type*> > nodes;
        nodes.push( std::make_pair( other.m_root, m_root ) );

        while( ! nodes.empty() )
        {
            const node_type * nother = nodes.front().first;
            node_type * nthis = nodes.front().second;
            nodes.pop();
            for( unsigned i = 0; i < children_size; ++i )
            {
                if( nother->children[i] )
                {
                    nthis->children[i] = new node_type( nthis );
                    nthis->children[i]->end = nother->children[i]->end;
                    nodes.push(
                        std::make_pair(
                            nother->children[i],
                            nthis->children[i]
                        )
                    );
                }
            }
        }
    }

    TrieTpl & operator=( TrieTpl other )
    {
        using std::swap;
        swap( *this, other );
        return *this;
    }

    friend void swap( TrieTpl & lhs, TrieTpl & rhs )
    {
        using std::swap;
        swap( lhs.m_root, rhs.m_root );
    }

    void insert( const std::string & word )
    {
        node_type * cur = m_root;
        for( unsigned i = 0; i < word.size(); ++i )
        {
            unsigned cidx = func_ctoi( word[i] );
            if( ! cur->children[cidx]  )
            {
                cur->children[cidx] = new node_type( cur );
            }
            cur = cur->children[cidx];
        }
        cur->end = true;
    }

    bool search( const std::string & word ) const
    {
        node_type * cur = m_root;
        for( auto i = 0; i < word.size(); ++i )
        {
            unsigned cidx = func_ctoi( word[i] );
            if( ! cur->children[ cidx ] )
            {
                return false;
            }
            cur = cur->children[ cidx ];
        }
        return nullptr != cur && cur->end;
    }

    const_iterator begin() const
    {
        return const_iterator{ m_root };
    }

    const_iterator end() const
    {
        return const_iterator{ nullptr };
    }

    const node_type * node() const
    {
        return m_root;
    }

private:
    node_type * m_root;
}; // end Trie

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

