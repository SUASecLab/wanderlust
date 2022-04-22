function init ()

end

function on_key_up ()
	wl_select_menu_entity ("DI")
end

function on_key_down ()
   wl_select_menu_entity ("HLT")
end

function on_key_left ()
	wl_select_menu_entity ("ANA")
end

function on_key_right ()
	wl_select_menu_entity ("NOP")
end

function on_key_a ()
	wl_mnemonic_set_base (0xFB)
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
      "ENABLE INTERRUPTS             " ..
      "                              " ..
      "The EI instruction enables the" ..
      "interrupt system following    " ..
      "execution of the next program " ..
      "instruction. Enabling the     " ..
      "interrupt system is delayed   " ..
      "one instruction to allow      " ..
      "interrupt subroutines to      " ..
      "return to the main program be-" ..
      "fore a subsequent interrupt is" ..
      "acknowledged.                 " ..
      "In applications that use      " ..
      "interrupts, the interrupt sys-" ..
      "tem is usually disabled only  " ..
      "when the processor accepts an " ..
		"interrupt or when a code      " ..
		"seqquence must not be inter-  " ..
		"rupted. You can disable the   " ..
		"interrupt system by including " ..
		"a D1 instruction at the begin-" ..
		"ning of the code sequence. Be-" ..
		"cause you cannot predict the  " ..
		"occurrence of an interrupt,   " ..
		"include an EI instruction at  " ..
		"the end of the code sequence. " ..
		"The EI instruction is freq-   " ..
		"quently used as part of a     " ..
		"start-up sequence. When power " ..
		"is first applied. The cpu     " ..
		"begins operating at some inde-" ..
		"terminate address. Application" ..
		"of a RESET signal forces the  " ..
		"program counter to zero. A    " ..
		"common instruction sequence at" ..
		"this point is El, HLT, These  " ..
		"instructions enable the inter-" ..
		"rupt system (RESET also dis-  " ..
		"ables the interrupt system)   " ..
		"and halt the processor. A sub-" ..
		"sequent manual or automatic   " ..
		"interrupt then determines the " ..
		"effective start-up address.   ",
      "frame.png"
   ) 
end

function on_key_start ()

end

function on_key_back ()

end

init ()
