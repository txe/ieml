#include "stdafx.h"
#include "../string_t.h"
#include <behaviors/behavior_aux.h>
#include "../json-aux-ext.h"


#pragma warning(disable:4786) //identifier was truncated...

namespace htmlayout 
{

	struct check_ball: public behavior
	{
		// ctor
		check_ball(): behavior(HANDLE_FOCUS | HANDLE_KEY | HANDLE_BEHAVIOR_EVENT, "check-ball") {}

		virtual BOOL on_key   (HELEMENT he, HELEMENT target, UINT event_type, UINT code, UINT keyboardStates ) 
		{ 
			dom::element t = target;

			if( event_type != KEY_DOWN )
				return FALSE;

			if( !aux::streq(t.get_element_type(),"td") )
				return FALSE;

			switch(code)
			{
				case VK_RETURN:
					return select_down_td( t );
				case VK_DOWN:
					return select_down_td( t );
				case VK_UP:
					return select_up_td( t );
			}
			return FALSE; 
		}

		BOOL select_down_td( dom::element& td )
		{	
			dom::element cur_tr = td.parent();
			dom::element next_tr = cur_tr.next_sibling();
			if(!next_tr.is_valid()) 
				return FALSE;
			if( !aux::streq(next_tr.get_element_type(),"tr") )
				return FALSE;
			cur_tr.set_state(0, STATE_CURRENT, false);
			next_tr.set_state(STATE_CURRENT);
			dom::element _td = next_tr.child(td.index());
			_td.set_state(STATE_FOCUS);
			dom::element(td.parent()).update(true);
			return TRUE;
		}

		BOOL select_up_td( dom::element& td )
		{
			dom::element cur_tr = td.parent();
			if (cur_tr.index() == 1)
				return FALSE;
			dom::element prev_tr = cur_tr.prev_sibling();
			if( !prev_tr.is_valid() ) 
				return FALSE;
			if( !aux::streq(prev_tr.get_element_type(),"tr") )
				return FALSE;
			cur_tr.set_state(0, STATE_CURRENT, false);
			prev_tr.set_state(STATE_CURRENT);
			dom::element _td = prev_tr.child(td.index());
			_td.set_state(STATE_FOCUS); 
			dom::element(td.parent()).update(true);
			return TRUE;
		}
		
		void check(HELEMENT t)
		{
			dom::element	td		= t;
			string_t		value	= td.get_text();
			double			ball	= aux::strtod(value, -1);
			string_t		atr		= L"good";
			if (value.empty())
				atr = L"";
			else
				if (ball < 2.5 || ball > 5.0)
					atr = L"bad";
			if (atr == string_t(L"good")) // необходимо для правильной прорисовки
				json::t2v(td, value);
			td.set_attribute("state", atr);
			td.update();
		}

		virtual BOOL on_focus  (HELEMENT he, HELEMENT target, UINT event_type )
		{ 
			if( event_type == (FOCUS_GOT | HANDLED))
			{
				dom::editbox td = he;
				td.select();
			}
			if( event_type == (FOCUS_LOST | HANDLED))
			{
				check(he);
			}
			return FALSE;
		}

		virtual BOOL on_event (HELEMENT he, HELEMENT target, BEHAVIOR_EVENTS type, UINT_PTR reason ) 
		{ 
			// LOG_DEBUG << type;
			if(type == CHECK_ALL)
			{	
				dom::element td = he;
				for(dom::element row = dom::element(td.parent()).first_sibling(); row.is_valid(); row = row.next_sibling())
					check(row.child(td.index()));
			}
			return FALSE;
		}
	};
	// instantiating and attaching it to the global list
	check_ball check_ball_instance;

} // htmlayout namespace

