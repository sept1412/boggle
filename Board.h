#pragma once

#include "Pos.h"
#include "utils.h"

#include <algorithm>
#include <iterator>
#include <memory>
#include <ostream>
#include <cstring>

namespace ppittawat::boggle
{

class Board
{
public:
    using row_t = pos_t::row_t;
    using col_t = pos_t::col_t;

    ~Board() = default;
    Board( Board && ) = default;
    Board & operator=( Board && ) = default;

    //running deep copy
    Board( const Board & other ):
        m_row( other.m_row ),
        m_col( other.m_col ),
        m_vals( new char[ m_row * m_col * 2 ] ) // each position contain 2 char
    {
        std::copy(
            other.m_vals.get(),
            other.m_vals.get() + ( other.m_row * other.m_col * 2 ),
            m_vals.get()
        );
    }

    Board & operator=( Board other )
    {
        using std::swap;
        swap( *this, other );
        return *this;
    }

    friend void swap( Board & lhs, Board & rhs )
    {
        using std::swap;
        swap( lhs.m_row, rhs.m_row );
        swap( lhs.m_col, rhs.m_col );
        swap( lhs.m_vals , rhs.m_vals );
    }

    //create 4x4 random board.
    Board():
        m_row( 4 ),
        m_col( 4 ),
        m_vals( new char[ m_row * m_col * 2 ] )
    {
        auto size = m_row * m_col;
        for( unsigned i = 0; i < size; ++i )
        {
            m_vals[i * 2] = random_letter();
            m_vals[i * 2 + 1] = '\0';
            if( m_vals[i*2] == 'Q' )
            {
                m_vals[ i*2 + 1 ] = 'U';
            }
        }
    }

    //init board with char array
    Board( const char * val, row_t row, col_t col ):
        m_row( row ),
        m_col( col ),
        m_vals( new char[ m_row * m_col * 2 ] )
    {
        std::memcpy( m_vals.get(), val, ( m_row * m_col * 2 ) );
    }

    size_t rows() const { return m_row; }

    size_t cols() const { return m_col; }

    std::string at( row_t r, col_t c ) const
    {
        std::string ret;
        unsigned vidx =  ( c + ( r * m_col ) ) * 2;
        ret.push_back( m_vals[ vidx ] );
        if( '\0' != m_vals[ vidx + 1 ] )
        {
            ret.push_back( m_vals[ vidx + 1] );
        }
        return ret;
    }

    std::string at( pos_t p ) const
    {
        return at( p.row, p.col );
    }

    friend std::ostream& operator<<( std::ostream & os, const Board & board )
    {
        os << "Board - row: "<< board.m_row << ", col: " << board.m_col << std::endl;
        for( row_t r = 0; r < board.m_row; ++r )
        {
            os << " ";
            for( col_t c = 0; c < board.m_col; ++c )
            {
                os << board.at( r, c ) << " ";
            }
            os << std::endl;
        }
        return os;
    }

    pos_t begin_pos() const
    {
        return { 0, 0 };
    }

    pos_t next_pos( pos_t cur ) const
    {
        if( cur.row == m_row -1 && cur.col == m_col -1 )
        {
            return end_pos();
        }

        pos_t next{ cur.row, cur.col + 1 };
        if( next.col >= m_col )
        {
            ++( next.row );
            next.col = 0;
        }
        return next;
    }

    pos_t end_pos() const
    {
        return { m_row, m_col };
    }

    //adjacent tiles
    pos_t lower_adj( pos_t cur ) const
    {
        return {
            ( cur.row > 0 ? cur.row - 1 : cur.row ),
            ( cur.col > 0 ? cur.col - 1 : cur.col )
        };
    }

    pos_t upper_adj( pos_t cur ) const
    {
        return {
            ( cur.row < m_row - 1 ? cur.row + 1 : cur.row ),
            ( cur.col < m_col - 1 ? cur.col + 1 : cur.col )
        };
    }

    // get start adjacent tile of the position cur
    pos_t begin_adj( pos_t cur ) const
    {
        return lower_adj( cur );
    }

    /**
     * get next adjacent tile of the position cur with the current adj is
     * adj
     */
    pos_t next_adj( pos_t cur, pos_t adj ) const
    {
        pos_t upper = upper_adj( cur );
        if( adj == upper )
        {
            return end_adj();
        }

        pos_t next{ adj.row, adj.col + 1 };

        if( next.col > upper.col )
        {
            next.col = lower_adj( cur ).col;
            ++( next.row );
        }

        return next;
    }

    pos_t end_adj() const
    {
        return { m_row, m_col };
    }

private:
    //number of row
    row_t m_row;

    //number of col
    col_t m_col;

    //leaner than std::vector or std::string and get the job done.
    std::unique_ptr<char[]> m_vals;
};

/*
 * @param valid format is that
 *      1st line contain 2 integer, Row and Column
 *      the next contain the letter of the board
 *      ** the file may have `Qu` as character but it will be store as 'Q'
 */
Board loadBoard( const char * file );

}//end namespace ppittawat::boggle

