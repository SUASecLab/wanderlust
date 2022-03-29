function init ()

end

function on_key_up ()
   wl_assembler_dcr_cursor ()
end

function on_key_down ()
   wl_assembler_inr_cursor ()
end

function on_key_left ()
end

function on_key_right ()
	wl_select_menu_entity ("TEXT")
end

function on_key_a ()
	wl_assembler_program_run ()
	wl_core_get_output ("door")
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
