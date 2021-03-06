
/* Copyright (c) 2011-2012, EPFL/Blue Brain Project
 *                     Stefan Eilemann <stefan.eilemann@epfl.ch>
 *
 * This file is part of DASH <https://github.com/BlueBrain/dash>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "change.h"

#include <dash/node.h>

namespace dash
{
namespace detail
{

Change::Change( dash::AttributePtr a )
        : type( ATTRIBUTE_CHANGED )
        , attribute( a )
{
}

Change::Change( const Type t, NodePtr p, dash::NodePtr c )
        : type( t )
        , node( p )
        , child( c )
{
}

Change::Change( const Type t, NodePtr p, dash::AttributePtr a )
        : type( t )
        , node( p )
        , attribute( a )
{
}

Change::Change( const Change& rhs )
    : type( rhs.type )
    , node( rhs.node )
    , child( rhs.child )
    , attribute( rhs.attribute )
{
}

bool Change::operator == ( const Change& rhs ) const
{
    if( this == &rhs )
        return true;

    if( type != rhs.type ||
        ( node && rhs.node && *node != *rhs.node ) ||
        ( child && rhs.child && *child != *rhs.child ) ||
        ( attribute && rhs.attribute && *attribute != *rhs.attribute ))
    {
        return false;
    }
    return true;
}


std::ostream& operator << ( std::ostream& os, const Change& change )
{
    return os << "Change " << change.type;
}

std::ostream& operator << ( std::ostream& os, const Change::Type& type )
{
    return os << ( type == Change::NONE ? "NONE" :
                   type == Change::NODE_INSERT ? "node insert" :
                   type == Change::NODE_ERASE ? "node erase" :
                   type == Change::ATTRIBUTE_INSERT ? "attribute insert" :
                   type == Change::ATTRIBUTE_ERASE ? "attribute erase" :
                   type == Change::ATTRIBUTE_CHANGED  ? "attribute change" :
                                                        "ERROR" );
}

}
}
