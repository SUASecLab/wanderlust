function init ()

end

function on_key_up ()
	wl_select_menu_entity ("b")   
end

function on_key_down ()
   wl_select_menu_entity ("t")
end

function on_key_left ()
	wl_select_menu_entity ("4")
end

function on_key_right ()
	wl_select_menu_entity ("6")
end

function on_key_a ()

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