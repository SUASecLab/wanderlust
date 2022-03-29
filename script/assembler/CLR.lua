function init ()

end

function on_key_up ()
   
end

function on_key_down ()
   
end

function on_key_left ()
	wl_select_menu_entity ("RM")
end

function on_key_right ()
	wl_select_menu_entity ("END")
end

function on_key_a ()

end

function on_key_b ()
	wl_undo_byte_terminal ()
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
