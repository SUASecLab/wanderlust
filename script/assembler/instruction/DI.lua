function init ()

end

function on_key_up ()
	wl_select_menu_entity ("DCX")
end

function on_key_down ()
   wl_select_menu_entity ("EI")
end

function on_key_left ()
	wl_select_menu_entity ("ADI")
end

function on_key_right ()
	wl_select_menu_entity ("MVI")
end

function on_key_a ()
	wl_mnemonic_set_base (0xF3)
	wl_repeal_menu ()
   wl_mnemonic_apply ()
end

function on_key_b ()
	wl_repeal_menu ()
end

function on_key_x ()

end

function on_key_y ()
   wl_create_textbox (
      "white", 
      "DISABLE INTERRUPTS            " ..
      "                              " ..
      "The interrupt system is dis-  " ..
      "abled when the processor re-  " ..
      "cognizes an interrupt or Im-  " .. 
      "mediately following execution " ..
		"of a DI instruction.          " ..
		"In applications that use      " ..
		"interrupts, the DI instruction" ..
		"is commonly used only when a  " ..
		"code sequence must not be     " ..
		"interrupted. For example, time" ..
		"dependent code sequences be-  " ..
		"come inaccurate when inter-   " ..
		"rupted. You can disable the   " ..
		"interrupt system by including " ..
		"a DI instruction at the be-   " ..
		"ginning of the code sequence. " ..
		"Because you cannot predict the" ..
		"occurrence of an interrupt,   " ..
		"include an EI instruction at  " ..
		"the end of the time-dependent " ..
		"code sequence.",
      "frame.png"
   ) 
end

function on_key_start ()

end

function on_key_back ()

end

init ()
