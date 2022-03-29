function init ()

end

function on_key_up ()
	wl_select_menu_entity ("IN")
end

function on_key_down ()
   wl_select_menu_entity ("INX")
end

function on_key_left ()
	wl_select_menu_entity ("CC")
end

function on_key_right ()
	wl_select_menu_entity ("OUT")
end

function on_key_a ()
   wl_mnemonic_set_base (0x04)
   wl_create_menu ("register", 176, 24, "frame.png")
   wl_create_entity (2, 176, 24, "b", "script/assembler/RS/B.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 32, "c", "script/assembler/RS/C.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 40, "d", "script/assembler/RS/D.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 48, "e", "script/assembler/RS/E.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 56, "h", "script/assembler/RS/H.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 64, "l", "script/assembler/RS/L.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 72, "m", "script/assembler/RS/m.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 80, "A", "script/assembler/RS/A.lua", "white", "red", "yellow", "gray")
   wl_append_menu ()
   wl_rundown_task ()
   wl_select_menu_entity ("b")
end

function on_key_b ()
	wl_repeal_menu ()
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
