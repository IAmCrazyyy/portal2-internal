#include "misc.h"
#include "../../../globals.h"

namespace hack::features::misc {
	void bhop( c_usercmd* cmd ) {
		if ( !( cmd->m_buttons & e_cmd_buttons::in_jump ) || 
			g::localplayer->m_fFlags() & e_ent_flags::fl_onground )
			return;

		cmd->m_buttons &= ~e_cmd_buttons::in_jump;
	}
}
