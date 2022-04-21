function init ()

end

function on_key_up ()
	wl_select_menu_entity ("ADC")
end

function on_key_down ()
   wl_select_menu_entity ("ADI")
end

function on_key_left ()
	wl_select_menu_entity ("RRC")
end

function on_key_right ()
	wl_select_menu_entity ("DCX")
end

function on_key_a ()
   wl_mnemonic_set_base (0x80)
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
      "ADD                           " ..
      "                              " ..
      "The ADD Instruction adds one  " ..
      "byte of data to the contents  " ..
      "of the accumulator. The result" ..
      "is stored in the accumulator  " ..
      "Notice that the ADD           " ..
      "instruction excludes the carry" ..
      "flag from the addition but    " ..
      "sets the flag to indicate the " ..
      "Outcome of the operation. The " ..
      "operand must specify one of   " ..
      "the registers A through E, H  " ..
      "or L. The instruction adds the" ..
      "contents of the specified     " ..
      "register to the contents of   " ..
      "the accumulator and stores the" ..
      "result in the accumulator.    " ..
      "This InstruCilon adds the con-" ..
      "tents of the memory location  " ..
      "addressed by the Hand L       " ..
      "registers to the contents of  " ..
      "the accumulator and stores the" ..
      "result in the accumulator. M  " ..
      "is a symbolic reference to the" ..
      "Hand L registers.", 
      "frame.png")
end

function on_key_start ()
	wl_run_terminal ()
end

function on_key_back ()
	wl_clear_terminal ()
end

init ()
