#pragma once

namespace ppittawat::boggle
{

struct pos_t
{
    //currently do not allow negative row, col
    using row_t = unsigned;
    using col_t = unsigned;

    bool operator==( const pos_t & other ) const
    {
        return row == other.row && col == other.col;
    }

    bool operator!=( const pos_t & other ) const
    {
        return ! ( *this == other );
    }

    row_t row;
    col_t col;
};

}
