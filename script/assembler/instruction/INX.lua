function init ()

end

function on_key_up ()
	wl_select_menu_entity ("INR")
end

function on_key_down ()
   wl_select_menu_entity ("JC")
end

function on_key_left ()
	wl_select_menu_entity ("CM")
end

function on_key_right ()
	wl_select_menu_entity ("PCHL")
end

function on_key_a ()
	wl_mnemonic_set_base (0x03)
	wl_create_menu ("byte_a", 176, 24, "frame.png")
	wl_create_entity (2, 176, 24, "BC", "script/assembler/RP/BC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 32, "DE", "script/assembler/RP/DE.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 40, "HL", "script/assembler/RP/HL.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 48, "SP", "script/assembler/RP/SP.lua", "white", "red", "yellow", "gray")
	wl_append_menu ()
	wl_rundown_task ()
	wl_select_menu_entity ("BC")
end

function on_key_x ()
	
end

function on_key_y ()
	wl_create_textbox (
      "white", 
      "INCREMENT REGISTER PAIR       " ..
      "                              " ..
      "The instruction adds one to   " ..
      "the contents of the specified " ..
      "register pair. It affects none" ..
      "of the condition flags. Be-   " ..
      "cause INX preserves all of the" ..
      "condition flags, it can be    " ..
      "used for address modification " ..
      "within multi-byte arithmetic  " ..
      "routines.                     " ..
      "INX may increment only the BC," ..
      "DE, HL or the 'Stack Pointer' " ..
      "(SP) register pairs.          " ..
      "Notice that the letter H must " ..
      "be used to specify the H and L" ..
      "register pair.",
      "frame.png"
   )
end

function on_key_start ()
	
end

function on_key_back ()
	
end

init ()
