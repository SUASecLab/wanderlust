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
	
end

function on_key_y ()
	wl_create_textbox (
      "white", 
      "DOUBLE REGISTER ADD           " ..
      "                              " ..
      "DAD adds the 16-bit value in  " ..
      "the specified register pair to" ..
      "the contents of the H and L   " ..
      "register pair. The result is  " ..
      "stored in H and L. DAD may add" ..
      "only the contents of the B&C, " ..
      "D&E, H&L, or the SP register  " ..
      "pairs to the contents of H, L." ..
      "Notice that the letter H must " ..
      "be used to specify that the   " ..
      "H and L register pair is to be" ..
      "added to Itself.              " ..
      "DAD sets the                  " ..
      "carry flag ON if there is a   " ..
      "carry out of the registers H, " ..
      "L. DAD will not affect any of " ..
      "the conditional flags other   " ..
      "than the carry flag.",
      "frame.png"
   ) 	
end

function on_key_start ()
	
end

function on_key_back ()
	
end

init ()
