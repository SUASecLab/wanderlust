function init ()

end

function on_key_up ()
	wl_select_menu_entity ("MOV")
end

function on_key_down ()
   	wl_select_menu_entity ("NOP")
end

function on_key_left ()
	wl_select_menu_entity ("DI")
end

function on_key_right ()
	wl_select_menu_entity ("RST")
end

function on_key_a ()
	wl_mnemonic_set_base (0x06)
	wl_create_menu ("pair", 176, 24, "frame.png")
	wl_create_entity (2, 176, 24, "b", "script/assembler/RB/register/B.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 32, "c", "script/assembler/RB/register/C.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 40, "d", "script/assembler/RB/register/D.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 48, "e", "script/assembler/RB/register/E.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 56, "h", "script/assembler/RB/register/H.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 64, "l", "script/assembler/RB/register/L.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 72, "m", "script/assembler/RB/register/m.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 80, "a", "script/assembler/RB/register/A.lua", "white", "red", "yellow", "gray")
	wl_append_menu ()
	wl_rundown_task ()
	wl_select_menu_entity ("b")
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
