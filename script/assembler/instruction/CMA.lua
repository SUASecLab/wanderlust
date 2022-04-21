function init ()

end

function on_key_up ()
	wl_select_menu_entity ("CM")
end

function on_key_down ()
   wl_select_menu_entity ("CMC")
end

function on_key_left ()
	wl_select_menu_entity ("STA")
end

function on_key_right ()
	wl_select_menu_entity ("JC")
end

function on_key_a ()
	wl_mnemonic_set_base (0x2F)
	wl_repeal_menu ()
   wl_mnemonic_apply ()
end

function on_key_x ()
	
end

function on_key_y ()
	wl_create_textbox (
      "white", 
      "COMPLEMENT ACCUMULATOR        " ..
      "                              " ..
  		"The instruction complements   " ..
  		"each bit of the accumulator to" ..
  		"produce it's one's complement." ..
  		"All condition flags remain un-" ..
  		"changed. To produce the two's " ..
  		"complement, add one to the    " ..
  		"contents of the accumulator   " ..
  		"after the CMA instructions has" ..
		"been executed.",
      "frame.png"
   )
end

function on_key_b ()
   wl_repeal_menu ()
end

function on_key_start ()
	
end

function on_key_back ()
	
end

init ()
