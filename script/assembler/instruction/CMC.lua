function init ()

end

function on_key_up ()
	wl_select_menu_entity ("CMA")
end

function on_key_down ()
   wl_select_menu_entity ("CMP")
end

function on_key_left ()
	wl_select_menu_entity ("STAX")
end

function on_key_right ()
	wl_select_menu_entity ("JM")
end

function on_key_a ()
	wl_mnemonic_set_base (0x3F)
	wl_repeal_menu ()
   wl_mnemonic_apply ()
end

function on_key_x ()
	
end

function on_key_y ()
	wl_create_textbox (
      "white", 
      "COMPLEMENT CARRY              " ..
      "                              " ..
  		"If the carry flag equals zero," ..
  		"CMC sets it to 1 and otherwise" ..
  		"to zero. All other flags will " ..
  		"remain unchanged.",
      "frame.png"
   )	
end

function on_key_start ()
	
end

function on_key_back ()
	
end

init ()
