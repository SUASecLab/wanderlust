function init ()

end

function on_key_up ()
   wl_assembler_dcr_cursor ()
end

function on_key_down ()
   wl_assembler_inr_cursor ()
end

function on_key_left ()
	wl_select_menu_entity ("TEXT")
	wl_terminal_text_mode ()
end

function on_key_right ()
	wl_select_menu_entity ("BACK")
end

function on_key_a ()
	Y = 16
	X = 16
	
	wl_create_menu ("BYTE", Y, X, "frame.png")

	for j = 0x0, 0xF do
		X = 16
		for i = 0x0, 0xF do
			wl_create_entity (
				2,  
				X,
				Y, 
				string.format ("%02X", 0xFF and (j * 16 + i)), 
				string.format ("script/assembler/table/%02X.lua", 0xFF and (j * 16 + i)),
				"mediumgray", "red", "yellow", "gray"
			)
			X = X + 0x10
		end
		Y = Y+8
	end

	wl_append_menu ()

	wl_rundown_task ()

	wl_select_menu_entity ("00")
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
