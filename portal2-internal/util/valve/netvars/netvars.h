#pragma once
#include "../../../sdk/misc/recv.h"
#include "../../mem/mem.h"
#include <utility>
#include <ostream>


// @credits: @markhc
namespace util::valve::netvars {
	struct netvar_item_t {
		std::string name;
		c_recv_prop* prop;
		std::uint32_t offset;
		std::vector<c_recv_prop*> child_props;
		std::vector<netvar_item_t> child_tables;
	};

	void capture( );
	std::uint32_t get( const std::string& table, const std::string& prop );
	c_recv_prop* get_prop( const std::string& table, const std::string& prop );
	void dump( );

	netvar_item_t _load_table( c_recv_table* client_cls );
	std::uint32_t _get_offset( const netvar_item_t& table, const std::string& prop );
	c_recv_prop* _get_prop( const netvar_item_t& table, const std::string& prop );
	void _dump_table( std::ostream& stream, const netvar_item_t& table, std::uint32_t indentation );

	inline std::vector<netvar_item_t> _items;
}
