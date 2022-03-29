function init ()

end

function on_key_up ()
	wl_select_menu_entity ("LXI")
end

function on_key_down ()
   	wl_select_menu_entity ("MVI")
end

function on_key_left ()
	wl_select_menu_entity ("DCX")
end

function on_key_right ()
	wl_select_menu_entity ("RRC")
end

function on_key_a ()
   wl_mnemonic_set_base (64)
   wl_create_menu ("register_a", 176, 24, "frame.png")
   wl_create_entity (2, 176, 24, "b", "script/assembler/RR/TARGET/B.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 32, "c", "script/assembler/RR/TARGET/C.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 40, "d", "script/assembler/RR/TARGET/D.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 48, "e", "script/assembler/RR/TARGET/E.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 56, "h", "script/assembler/RR/TARGET/H.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 64, "l", "script/assembler/RR/TARGET/L.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 72, "m", "script/assembler/RR/TARGET/m.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 80, "A", "script/assembler/RR/TARGET/A.lua", "white", "red", "yellow", "gray")
   wl_append_menu ()
   wl_rundown_task ()
   wl_select_menu_entity ("b")
end

function on_key_b ()

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
