namespace ppittawat::trie {

/**
 * Implementation TrieNodeTpl
 */

template <
    size_t children_size
    , func_char_to_uint func_ctoi
    , func_uint_to_char func_itoc
>
TrieNodeTpl<children_size, func_ctoi, func_itoc>::
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

template <
    size_t children_size
    , func_char_to_uint func_ctoi
    , func_uint_to_char func_itoc
>
TrieNodeTpl<children_size, func_ctoi, func_itoc>::
TrieNodeTpl():
    parent( nullptr ),
    children(),
    end( false )
{}

template <
    size_t children_size
    , func_char_to_uint func_ctoi
    , func_uint_to_char func_itoc
>
TrieNodeTpl<children_size, func_ctoi, func_itoc>::
TrieNodeTpl( TrieNodeTpl * _parent ):
    parent( _parent ),
    children(),
    end( false )
{}

template <
    size_t children_size
    , func_char_to_uint func_ctoi
    , func_uint_to_char func_itoc
>
auto TrieNodeTpl<children_size, func_ctoi, func_itoc>::
child( char c ) -> TrieNodeTpl *
{
    return children[ func_ctoi( c ) ];
}

template <
    size_t children_size
    , func_char_to_uint func_ctoi
    , func_uint_to_char func_itoc
>
auto TrieNodeTpl<children_size, func_ctoi, func_itoc>::
child( char c ) const -> const TrieNodeTpl *
{
    return children[ func_ctoi( c ) ];
}

template <
    size_t children_size
    , func_char_to_uint func_ctoi
    , func_uint_to_char func_itoc
>
auto TrieNodeTpl<children_size, func_ctoi, func_itoc>::
child( unsigned i ) -> TrieNodeTpl *
{
    return children[ i ];
}

template <
    size_t children_size
    , func_char_to_uint func_ctoi
    , func_uint_to_char func_itoc
>
auto TrieNodeTpl<children_size, func_ctoi, func_itoc>::
child( unsigned i ) const -> const TrieNodeTpl *
{
    return children[ i ];
}

/**
 * Implementation TrieTpl::const_iterator
 */

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
TrieTpl< children_size, func_ctoi, func_itoc >::const_iterator::
const_iterator( const node_type * root ):
    m_root( root ),
    m_cur( root ),
    m_val()
{
    //break at the first word
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

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
auto TrieTpl< children_size, func_ctoi, func_itoc >::const_iterator::
operator++() -> const_iterator &
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

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
auto TrieTpl< children_size, func_ctoi, func_itoc >::const_iterator::
operator++(int) -> const_iterator
{
    const_iterator retval = *this;
    ++(*this);
    return retval;
}

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
auto TrieTpl< children_size, func_ctoi, func_itoc >::const_iterator::
operator==( const const_iterator & other ) const -> bool
{
    return (
        ( m_root == other.m_root )
        && ( m_cur == other.m_cur )
    );
}

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
auto TrieTpl< children_size, func_ctoi, func_itoc >::const_iterator::
operator!=( const const_iterator & other ) const -> bool
{
    return ! ( *this == other );
}

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
auto TrieTpl< children_size, func_ctoi, func_itoc >::const_iterator::
operator*() const -> reference
{
    return m_val;
}

/**
 * Implementation TrieTpl
 */
template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
TrieTpl< children_size, func_ctoi, func_itoc >::
~TrieTpl()
{
    if( m_root )
    {
        delete m_root;
    }
}

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
TrieTpl< children_size, func_ctoi, func_itoc >::
TrieTpl( TrieTpl && other ):
    m_root( other.m_root )
{
    //simply steal the resource
    other.m_root = nullptr;
}

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
TrieTpl< children_size, func_ctoi, func_itoc >::
TrieTpl():
    m_root( new node_type )
{
}

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
TrieTpl< children_size, func_ctoi, func_itoc >::
TrieTpl( const TrieTpl & other ):
    m_root( new node_type )
{
    //need a deep copy here using BFS Traversal
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

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
auto TrieTpl< children_size, func_ctoi, func_itoc >::
operator=( TrieTpl other ) -> TrieTpl &
{
    using std::swap;
    swap( *this, other );
    return *this;
}

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
auto TrieTpl< children_size, func_ctoi, func_itoc >::
insert( const std::string & word ) -> void
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

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
auto TrieTpl< children_size, func_ctoi, func_itoc >::
search( const std::string & word ) const -> bool
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

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
auto TrieTpl< children_size, func_ctoi, func_itoc >::
begin() const -> const_iterator
{
    return const_iterator{ m_root };
}

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
auto TrieTpl< children_size, func_ctoi, func_itoc >::
end() const -> const_iterator
{
    return const_iterator{ nullptr };
}

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
auto TrieTpl< children_size, func_ctoi, func_itoc >::
node() const -> const node_type *
{
    return m_root;
}

template <
    size_t children_size,
    func_char_to_uint func_ctoi,
    func_uint_to_char func_itoc
>
auto swap
(
    TrieTpl< children_size, func_ctoi, func_itoc > & lhs,
    TrieTpl< children_size, func_ctoi, func_itoc >  & rhs
) -> void
{
    using std::swap;
    swap( lhs.m_root, rhs.m_root );
}

} // end namespace ppittawat::trie
