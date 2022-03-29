function init ()

end

function on_key_up ()
	wl_select_menu_entity ("RC")
end

function on_key_down ()
   	wl_select_menu_entity ("RLC")
end

function on_key_left ()
	wl_select_menu_entity ("JP")
end

function on_key_right ()
	wl_select_menu_entity ("XCHG")
end

function on_key_a ()
	wl_mnemonic_set_base (0xC9)
	wl_repeal_menu ()
	wl_mnemonic_apply ()
end

function on_key_x ()
	wl_increment_cursor_terminal ()
end

function on_key_y ()
	wl_decrement_cursor_terminal ()
end

function on_key_start ()
	wl_run_terminal ()
end

function on_key_back ()
	wl_clear_terminal ()
end

init ()
