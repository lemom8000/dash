
/* Copyright (c) 2011, EPFL/Blue Brain Project
 *                     Stefan.Eilemann@epfl.ch
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

#include "attribute.h"

#include "detail/attribute.h"
#include "detail/serializable.h"

namespace dash
{

SERIALIZABLETEXTARCHIVE( Attribute, SERIALIZABLEREF )
SERIALIZABLEBINARYARCHIVE( Attribute, SERIALIZABLEREF )

Attribute::Attribute()
    : Referenced()
#pragma warning( push )
#pragma warning( disable: 4355 )
    , impl_( new detail::Attribute( this ))
#pragma warning( pop )
{
    impl_->ref( this );
}

Attribute::Attribute( const Attribute& from )
        : Referenced( from )
#pragma warning( push )
#pragma warning( disable: 4355 )
        , impl_( new detail::Attribute( this, from.impl_ ))
#pragma warning( pop )
{
    impl_->ref( this );
}

void Attribute::init_()
{
    impl_ = new detail::Attribute( this );
    impl_->ref( this );
}

Attribute::~Attribute()
{
    impl_->orphan();
    impl_->unref( this );
}

Attribute& Attribute::operator = ( const Attribute& from )
{
    if( this == &from )
        return *this;
    *impl_ = *from.impl_;
    return *this;
}

bool Attribute::operator == ( const Attribute& rhs ) const
{
    if( this == &rhs )
        return true;
    return *impl_ == *rhs.impl_;
}

Attribute& Attribute::set_( const lunchbox::Any& value )
{
    impl_->set( value );
    return *this;
}

lunchbox::Any& Attribute::getMutable_()
{
    return impl_->getMutable();
}

const lunchbox::Any& Attribute::get_() const
{
    return impl_->get();
}


}
