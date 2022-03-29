function init ()

end

function on_key_up ()
	wl_select_menu_entity ("LDAX")
end

function on_key_down ()
   wl_select_menu_entity ("DCR")
end

function on_key_left ()
	wl_select_menu_entity ("ACI")
end

function on_key_right ()
	wl_select_menu_entity ("LHLD")
end

function on_key_a ()
	wl_mnemonic_set_base (0x09)
	wl_create_menu ("pair", 176, 24, "frame.png")
	wl_create_entity (2, 176, 24, "BC", "script/assembler/RP/BC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 32, "DE", "script/assembler/RP/DE.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 40, "HL", "script/assembler/RP/HL.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 48, "SP", "script/assembler/RP/SP.lua", "white", "red", "yellow", "gray")
	wl_append_menu ()
	wl_rundown_task ()
	wl_select_menu_entity ("BC")
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
