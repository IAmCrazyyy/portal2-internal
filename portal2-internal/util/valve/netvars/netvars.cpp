#include "netvars.h"
#include "../../../sdk/portal.h"
#include <fstream>


namespace util::valve::netvars {
	void capture( ) {
		_items.clear( );
		for ( auto cls = portal::interfaces::m_hl_client->GetAllClasses( ); cls != nullptr; cls = cls->m_next ) {
			if ( !cls->m_recv_table )
				continue;

			_items.emplace_back( _load_table( cls->m_recv_table ) );
		}
	}

	std::uint32_t get( const std::string& table, const std::string& prop ) {
		for ( const auto& t : _items ) {
			if ( t.name != table )
				continue;

			auto result = _get_offset( t, prop );
			if ( result == 0 )
				continue;

			return result;
		}
		return 0;
	}

	c_recv_prop* get_prop( const std::string& table, const std::string& prop ) {
		c_recv_prop* result = nullptr;

		for ( const auto& t : _items ) {
			if ( t.name != table )
				continue;

			result = _get_prop( t, prop );
		}

		return result;
	}

	void dump( ) {
		auto stream = std::ofstream( "netvar_dump.txt" );

		for ( const auto& table : _items ) {
			if ( table.child_props.empty( ) && table.child_tables.empty( ) )
				continue;

			stream << table.name << '\n';
			_dump_table( stream, table, 1 );
			stream << '\n';
		}

		stream << std::endl;
		stream.close( );
	}

	netvar_item_t _load_table( c_recv_table* client_cls ) {
		auto table = netvar_item_t {};

		table.offset = 0;
		table.name = client_cls->m_net_tablename;

		for ( auto i = 0; i < client_cls->m_propscount; ++i ) {
			auto prop = &client_cls->m_props[ i ];

			if ( !prop || isdigit( prop->m_varname[ 0 ] ) )
				continue;

			if ( !strcmp( "baseclass", prop->m_varname ) )
				continue;

			if ( prop->m_recvtype != 6 /* not datatable */ || !prop->m_datatable )
				continue;

			table.child_tables.emplace_back( _load_table( prop->m_datatable ) );
			table.child_tables.back( ).offset = prop->m_offset;
			table.child_tables.back( ).prop = prop;
		}
		return table;
	}

	std::uint32_t _get_offset( const netvar_item_t& table, const std::string& prop ) {
		for ( const auto& p : table.child_props ) {
			if ( strncmp( p->m_varname, prop.data( ), prop.size( ) ) != 0 )
				continue;

			return table.offset + p->m_offset;
		}
		for ( const auto& child : table.child_tables ) {
			auto prop_offset = _get_offset( child, prop );

			if ( prop_offset == 0 )
				continue;

			return table.offset + prop_offset;
		}
		for ( const auto& child : table.child_tables ) {
			if ( strncmp( child.prop->m_varname, prop.data( ), prop.size( ) ) != 0 )
				continue;

			return table.offset + child.offset;
		}
		return 0;
	}

	c_recv_prop* _get_prop( const netvar_item_t& table, const std::string& prop ) {
		for ( const auto& p : table.child_props ) {
			if ( strncmp( p->m_varname, prop.data( ), prop.size( ) ) != 0 )
				continue;

			return p;
		}
		for ( const auto& child : table.child_tables ) {
			auto p = _get_prop( child, prop );

			if ( p == 0 )
				continue;

			return p;
		}
		for ( const auto& child : table.child_tables ) {
			if ( strncmp( child.prop->m_varname, prop.data( ), prop.size( ) ) != 0 )
				continue;

			return child.prop;
		}
		return nullptr;
	}

	void _dump_table( std::ostream& stream, const netvar_item_t& table, std::uint32_t indentation ) {
		char line_buffer[ 1024 ];

		for ( const auto& prop : table.child_props ) {
			sprintf_s( line_buffer, "%*c%*s: 0x%08X", indentation * 4, ' ', -( 50 - static_cast< int >( indentation ) * 4 ), prop->m_varname, table.offset + prop->m_offset );
			stream << line_buffer << '\n';
		}
		for ( const auto& child : table.child_tables ) {
			sprintf_s( line_buffer, "%*c%*s: 0x%08X", indentation * 4, ' ', -( 50 - static_cast< int >( indentation ) * 4 ), child.prop->m_varname, table.offset + child.offset );
			stream << line_buffer << '\n';
			_dump_table( stream, child, indentation + 1 );
		}
	}
}
