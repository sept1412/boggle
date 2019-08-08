#include "Solver.h"
#include "Pos.h"

#include "debug.h"

#include <iostream>
#include <vector>

using namespace std;

namespace ppittawat::boggle
{

using row_t = pos_t::row_t;
using col_t = pos_t::col_t;
using node_type = Dictionary::node_type;

class BoardStack
{
public:
    BoardStack( row_t r, col_t c ):
        m_rows( r ),
        m_cols( c ),
        m_isIn( r * c, false )
    {
    }

    bool has( row_t r, col_t c ) const
    {
        return m_isIn[ c + ( r * m_cols ) ];
    }

    bool has( pos_t p ) const
    {
        return has( p.row, p.col );
    }

    void push( row_t r, col_t c, char ch )
    {
        m_isIn[ c + ( r * m_cols ) ] = true;
        m_pos.push_back( pos_t{ r, c } );
        m_word.push_back( ch );
    }

    void push( pos_t p, char ch )
    {
        push( p.row, p.col, ch );
    }

    void push( row_t r, col_t c, const string & s )
    {
        m_isIn[ c + ( r * m_cols ) ] = true;
        m_pos.push_back( pos_t{ r, c } );
        for( unsigned i = 0; i < s.size(); ++i )
            m_word.push_back( s[i] );
    }

    void push( pos_t p, const string & s )
    {
        push( p.row, p.col, s );
    }

    bool empty() const
    {
        return m_pos.empty();
    }

    row_t topRow() const
    {
        return m_pos.back().row;
    }

    col_t topCol() const
    {
        return m_pos.back().col;
    }

    pos_t topPos() const
    {
        return m_pos.back();
    }

    const string & word() const
    {
        return m_word;
    }

    void pop()
    {
        m_isIn[ topCol() + ( topRow() * m_cols ) ] = false;
        m_pos.pop_back();
        m_word.pop_back();
    }

    void pop( const string & s )
    {
        m_isIn[ topCol() + ( topRow() * m_cols ) ] = false;
        m_pos.pop_back();
        for( unsigned i = 0; i < s.size(); ++i )
            m_word.pop_back();
    }

    row_t rows() const{ return m_rows; }
    col_t cols() const{ return m_cols; }

private:
    // number of rows
    row_t m_rows;

    // number of cols
    col_t m_cols;

    /**
     * To check which ( r, c ) is in the stack.in O(1)
     * In contrast to search the pos to check which (r,c) is in here stack.
     * - for convenience -
     */
    vector<bool> m_isIn;

    /**
     * actual stack position of a board that is processed.
     */
    vector< pos_t > m_pos;

    /**
     * current word traverse
     * In contrast to track the Trie node back to parent and recreate a word.
     * - for convenience -
     */
    string m_word;
};

bool match( const node_type * node, const string & s )
{
    const node_type * cur = node;
    for( unsigned i = 0; i < s.size(); ++i )
    {
        if( ! cur->child( s[i] ) )
        {
            return false;
        }
        cur = cur->child( s[i] );
    }
    return nullptr != cur ;
}

set< string > Solver::getAllValidWords( const Board & board ) const
{
    /**
     * all result saved here.
     */
    set< string > ret;

    const node_type * root  = m_dict.node();

    //TODO: ADD SPECIAL QU HANDLE
    //running DFS all tiles as root

    for( pos_t p = board.begin_pos()
        ; p != board.end_pos()
        ; p = board.next_pos( p ) )
    {
        dlog << "Start - r: " << p.row << ", c: " << p.col << endl;

        const node_type * cur = root;

        //not found the first one then skip
        //if( ! cur->child( board.at( p ) ) )
        if( ! match( cur, board.at( p ) ) )
        {
            dlog
                << " | current (" << board.at( p ).c_str()
                << ") is not in the dict" << endl;
            continue;
        }

        //TODO move to into another func.
        //create the stack to run DFS
        dlog << " | add " << board.at( p ).c_str() << endl << endl;

        BoardStack stack( board.rows(), board.cols() );

        string w = board.at( p );
        stack.push( p, w );
        for( unsigned i = 0; i < w.size(); ++i )
        {
            cur = cur->child( w[i] );
        }
        pos_t cpos = p;
        pos_t adj = board.begin_adj( cpos );

        while( ! stack.empty() )
        {
            //loop adj node
            while( board.end_adj() != adj )
            {
                //do not allow dup node
                if( stack.has( adj ) )
                {
                    adj = board.next_adj( cpos, adj );
                    continue;
                }

                dlog
                    << " cpos ( " << cpos.row << ", " << cpos.col << " )"
                    << ", adj ( " << adj.row << ", " << adj.col << " ):"
                    << board.at( adj ).c_str() << " word "
                    << stack.word() << endl;

                //if found, then add and reset adj
                if( match( cur, board.at( adj ) ) )
                {
                    dlog << " | found : " << board.at( adj ).c_str();

                    string w = board.at( adj );
                    stack.push( adj, w );
                    for( unsigned i = 0; i < w.size(); ++i )
                    {
                        cur = cur->child( w[i] );
                    }

                    cpos = adj;
                    adj = board.begin_adj( cpos );

                    if( cur->end )
                    {
                        dlog << " >> add "<< stack.word() << " as a word ";
                        ret.emplace( stack.word() );
                    }

                    dlog << endl << endl;
                }
                else
                {
                    adj = board.next_adj( cpos, adj );
                }
            }

            //if all adj is not found then pop the previous state.
            adj = stack.topPos();
            string pw = board.at( adj );
            for( unsigned i = 0; i < pw.size(); ++i )
                cur = cur->parent;
            stack.pop( pw );
            cpos = stack.topPos();

            //move adj to the next one
            adj = board.next_adj( cpos, adj );

            dlog
                << " | pop, cpos( " << cpos.row << ", " << cpos.col << " )"
                << " word " << stack.word() << endl;
        }// while( ! stack.empty() )
    } // for( pos_t p = board.begin_pos()

    return  ret;
}

} // end namespace ppittawat::boggle
