function init ()

end

function on_key_up ()
	wl_select_menu_entity ("CZ")
end

function on_key_down ()
   wl_select_menu_entity ("ACI")
end

function on_key_left ()
	wl_select_menu_entity ("back")
end

function on_key_right ()
	wl_select_menu_entity ("LDAX")
end

function on_key_a ()
	wl_mnemonic_set_base (0x27)
	wl_repeal_menu ()
   wl_mnemonic_apply ()
end

function on_key_x ()

end

function on_key_y ()
	wl_create_textbox (
      "white", 
      "DECIMAL ADJUST ACCUMULATOR    " ..
      "                              " ..
      "The instruction adjusts the   " ..
		"8-bit value in the accumulator" ..
		"to form two 4-bit binary coded" ..
		"decimal digits.",
      "frame.png"
   ) 	
end

function on_key_start ()

end

function on_key_back ()

end

init ()
