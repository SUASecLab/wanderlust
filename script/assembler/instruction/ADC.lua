function init ()

end

function on_key_up ()
	wl_select_menu_entity ("ACI")
end

function on_key_down ()
   wl_select_menu_entity ("ADD")
end

function on_key_left ()
	wl_select_menu_entity ("RPO")
end

function on_key_right ()
	wl_select_menu_entity ("DCR")
end

function on_key_a ()
   wl_mnemonic_set_base (0x88)
   wl_create_menu ("register", 176, 24, "frame.png")
   wl_create_entity (2, 176, 24, "b", "script/assembler/RS/B_.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 32, "c", "script/assembler/RS/C_.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 40, "d", "script/assembler/RS/D_.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 48, "e", "script/assembler/RS/E_.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 56, "h", "script/assembler/RS/H_.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 64, "l", "script/assembler/RS/L_.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 72, "m", "script/assembler/RS/m_.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 80, "A", "script/assembler/RS/A_.lua", "white", "red", "yellow", "gray")
   wl_append_menu ()
   wl_rundown_task ()
   wl_select_menu_entity ("b")
end

function on_key_b ()
   wl_repeal_menu ()
end

function on_key_x ()
	
end

function on_key_y ()
	  wl_create_textbox (
      "white", 
      "ADD WITH CARRY                " ..
      "                              " ..
      "The ADC instruction adds one  " ..
      "byte of data plus the setting " ..
      "of the carry flag to the con- " ..
      "tents of the accumulator. The " ..
      "result istored in the acc-    " ..
      "umulator, ADC then updates the" ..
      "setting of the carry flag to  " ..
      "indicate the outcome of the   " ..
      "operaton. The ADC innuction's " ..
      "use of the carry bit enables  " ..
      "the program to add multi-byte " ..
      "numeric strings. This         " ..
      "instruction adds the contents " ..
      "of the memory location        " ..
      "addressed by the Hand L       " ..
      "registers and the carry bit to" ..
      "the accumulator and stores the" ..
      "result in the accumulator. M  " ..
      "is a symbolic reference to the" ..
      "Hand L registers.", 
      "frame.png")
end

function on_key_start ()
	
end

function on_key_back ()
	
end

init ()
