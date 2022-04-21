function init ()

end

function on_key_up ()
	wl_select_menu_entity ("ADI")
end

function on_key_down ()
   wl_select_menu_entity ("ANI")
end

function on_key_left ()
	wl_select_menu_entity ("RZ")
end

function on_key_right ()
	wl_select_menu_entity ("EI")
end

function on_key_a ()
   wl_mnemonic_set_base (0xA0)
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
      "LOGICAL AND WITH ACCUMULATOR  " ..
      "                              " ..
      "ANA performs a logical AND    " ..
      "operation, using the contents " ..
      "of the specified byte and the " ..
      "accumulator. The result is    " ..
      "placed in tlie accumulator. It" ..
      "produces a one bit In the     " ..
      "result only when the corres-  " ..
      "ponding bits In the test data " ..
      "and the mask data are ones.   " ..
      "The operand must specify one  " ..
      "of the registers A through E, " ..
      "H or L. This instruction ANDs " ..
      "the contents of the specified " ..
      "register with the accumulator " ..
      "and stores the result in the  " ..
      "accumulator, The carry flag is" ..
      "reset to zero. With 'm', this " ..
      "Instruction ANDs the contents " ..
      "of the specified memory       " ..
      "location with the accumulator " ..
      "and stores the result in the  " ..
      "accumulator. The carry flag is" ..
      "reset to zero.",
      "frame.png"
   )
end

function on_key_start ()
	
end

function on_key_back ()
	
end

init ()
