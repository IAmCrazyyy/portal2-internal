#pragma once
#include "../util/util.h"
#include "interfaces/interfaces.h"
#include "enums/enums.h"


namespace portal {
	namespace modules {
		inline util::mem::module_t m_client;
		inline util::mem::module_t m_engine;
		inline util::mem::module_t m_gameoverlayrenderer;

		void capture( );
	}

	namespace patterns {
		inline util::mem::addr_t m_present;
		inline util::mem::addr_t m_reset;

		void capture( );
	}

	namespace interfaces {
		inline c_engine_client* m_engine_client;
		inline c_entitylist* m_entitylist;
		inline c_hl_client* m_hl_client;
		inline c_input* m_input;

		void capture( );
		void _dump( );
	}

	void initial( );
}
