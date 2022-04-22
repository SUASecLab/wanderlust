function init ()

end

function on_key_up ()
	wl_select_menu_entity ("EI")
end

function on_key_down ()
   wl_select_menu_entity ("IN")
end

function on_key_left ()
	wl_select_menu_entity ("ANI")
end

function on_key_right ()
	wl_select_menu_entity ("ORA")
end

function on_key_a ()
	wl_mnemonic_set_base (0x76)
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
      "HALT                          " ..
      "                              " ..
      "The HLT instruction halts the " ..
      "processor. The program counter" ..
      "contains the address of the   " ..
      "next sequential instruction.  " ..
      "Otherwise, the flags and re-  " ..
      "gisters remain unchanged. Once" ..
      "in the halt state, the cpu can" ..
      "be restarted only by an       " ..
      "external event, typically an  " ..
      "interrupt. Therefore, you     " ..
      "should be certain that inter- " ..
      "rupts are enabled before the  " ..
      "HLT instruction is executed.  " ..
      "See the description of the EI " ..
      "instruction.                  " ..
      "If an HLT instruction is exe- " ..
      "cuted while interrupts are    " ..
      "disabled, the only way to re- " ..
      "start the processor is by app-" ..
      "plication of a RESET signal   " ..
      "and forces the program counter" ..
      "to zero. The same is true of  " ..
      "the 8085, except for the TRAP " ..
      "Interrupt, which IS recognized" ..
      "even when the interrupt system" ..
      "is disabled.                  " ..
      "The proceSSJr can temporarily " ..
      "leave the HLT state to service" ..
      "a direct mem access request.  " ..
      "However, the processor re-    " ..
      "enters the halt state once the" ..
      "request has been serviced.    " ..
      "A basic purpose for the HLT   " ..
      "instruction is to allow the   " ..
      "processor to pause while wait-" ..
      "ing for an interrupt from a   " ..
		"peripheral device. However, a " ..
		"HLT wastes processor resources" ..
		"and should be used only when  " ..
		"there is no useful processing " ..
		"task available.",
      "frame.png"
   ) 
end

function on_key_start ()

end

function on_key_back ()

end

init ()
