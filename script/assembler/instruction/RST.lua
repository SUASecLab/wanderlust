function init ()

end

function on_key_up ()
	wl_select_menu_entity ("RRC")
end

function on_key_down ()
   wl_select_menu_entity ("RZ")
end

function on_key_left ()
	wl_select_menu_entity ("MVI")
end

function on_key_right ()
	wl_select_menu_entity ("ADI")
end

function on_key_a ()
   wl_mnemonic_set_base (0xC7)
   wl_create_menu ("register", 176, 24, "frame.png")
   wl_create_entity (2, 176, 24, "0", "script/assembler/Constant/0.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 32, "1", "script/assembler/Constant/1.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 40, "2", "script/assembler/Constant/2.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 48, "3", "script/assembler/Constant/3.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 56, "4", "script/assembler/Constant/4.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 64, "5", "script/assembler/Constant/5.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 72, "6", "script/assembler/Constant/6.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 80, "7", "script/assembler/Constant/7.lua", "white", "red", "yellow", "gray")
   wl_append_menu ()
   wl_rundown_task ()
   wl_select_menu_entity ("0")
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
