
/* Copyright (c) 2012, EFPL/Blue Brain Project
 *                     Daniel Nachbaur <danie.nachbaur@epfl.ch>
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

#include "test.h"
#include "serialize.h"

#include <co/co.h>
#include <dash/dash.h>
#include <dash/co/Serialization.h>

#include <boost/serialization/vector.hpp>
SERIALIZABLEANY( std::vector<float> )


class AttributeDist : public co::Object
{
public:
    AttributeDist()
        : _attribute( new dash::Attribute )
    {}

    AttributeDist( dash::AttributePtr attr )
        : _attribute( attr )
    {}

    const dash::AttributePtr getAttribute() const
    {
        return _attribute;
    }

protected:
    virtual void getInstanceData( co::DataOStream& os )
    {
        dash::detail::serializeAny< co::DataOStreamArchive >( *_attribute, os );
    }

    virtual void applyInstanceData( co::DataIStream& is )
    {
        dash::detail::serializeAny< co::DataIStreamArchive >( *_attribute, is );
    }

private:
    dash::AttributePtr _attribute;
};


template< typename T >
void testAttributeSerialization( co::LocalNodePtr server,
                                 co::LocalNodePtr client, const T& value )
{
    dash::AttributePtr attribute = new dash::Attribute( value );

    AttributeDist attributeDist( attribute );
    TEST( server->registerObject( &attributeDist ));

    AttributeDist remoteAttributeDist;
    TEST( client->mapObject( &remoteAttributeDist, attributeDist.getID( )));

    TEST( attribute->get<T>() ==
          remoteAttributeDist.getAttribute()->get<T>( ));

    client->unmapObject( &remoteAttributeDist );
    server->deregisterObject( &attributeDist );
}


int dash::test::main( int argc, char **argv )
{
    TEST( co::init( argc, argv ));

    co::ConnectionDescriptionPtr connDesc = new co::ConnectionDescription;

    co::base::RNG rng;
    connDesc->type = co::CONNECTIONTYPE_TCPIP;
    connDesc->port = (rng.get<uint16_t>() % 60000) + 1024;
    connDesc->setHostname( "localhost" );

    co::LocalNodePtr server = new co::LocalNode;
    server->addConnectionDescription( connDesc );
    TEST( server->listen( ));

    co::NodePtr serverProxy = new co::Node;
    serverProxy->addConnectionDescription( connDesc );

    connDesc = new co::ConnectionDescription;
    connDesc->type = co::CONNECTIONTYPE_TCPIP;
    connDesc->setHostname( "localhost" );

    co::LocalNodePtr client = new co::LocalNode;
    client->addConnectionDescription( connDesc );
    TEST( client->listen( ));
    TEST( client->connect( serverProxy ));

    testAttributeSerialization( server, client, 42 );
    testAttributeSerialization( server, client, 5.f );
    testAttributeSerialization( server, client, false );
    testAttributeSerialization( server, client, std::string( "blablub" ));
    testAttributeSerialization( server, client, co::uint128_t( 12345, 54321 ));
    Foo foo = {42, 1.5f, false, "blablub"};
    testAttributeSerialization( server, client, foo );
    std::vector< float > values( 10, 2.34f );
    testAttributeSerialization( server, client, values );

    TEST( client->disconnect( serverProxy ));
    TEST( client->close( ));
    TEST( server->close( ));

    serverProxy->printHolders( std::cerr );
    TESTINFO( serverProxy->getRefCount() == 1, serverProxy->getRefCount( ));
    TESTINFO( client->getRefCount() == 1, client->getRefCount( ));
    TESTINFO( server->getRefCount() == 1, server->getRefCount( ));

    serverProxy = 0;
    client      = 0;
    server      = 0;

    TEST( co::exit( ));

    return EXIT_SUCCESS;
}
