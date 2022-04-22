function init ()

end

function on_key_up ()
	wl_select_menu_entity ("DCR")
end

function on_key_down ()
   wl_select_menu_entity ("DI")
end

function on_key_left ()
	wl_select_menu_entity ("ADD")
end

function on_key_right ()
	wl_select_menu_entity ("MOV")
end

function on_key_a ()
	wl_mnemonic_set_base (0x0B)
	wl_create_menu ("byte_a", 176, 24, "frame.png")
	wl_create_entity (2, 176, 24, "BC", "script/assembler/RP/BC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 32, "DE", "script/assembler/RP/DE.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 40, "HL", "script/assembler/RP/HL.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 48, "SP", "script/assembler/RP/SP.lua", "white", "red", "yellow", "gray")
	wl_append_menu ()
	wl_rundown_task ()
	wl_select_menu_entity ("BC")
end

function on_key_b ()
	wl_repeal_menu ()
end

function on_key_x ()

end

function on_key_y ()
   wl_create_textbox (
      "white", 
      "DECREMENT REGISTER PAIR       " ..
      "                              " ..
      "DCX decrements the contents of" ..
      "the specified register pair by" ..
      "one.                          " .. 
      "DCX does not affect any of the" ..
      "conditional flags.            " ..
		"Because DCX does preserves all" ..
		"of the flags, it can be used  " ..
		"for address modification in   " ..
		"any instruction sequence that " ..
		"relies on the passing of the  " ..
		"flags.",
      "frame.png"
   ) 
end

function on_key_start ()

end

function on_key_back ()

end

init ()
