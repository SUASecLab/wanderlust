function init ()

end

function on_key_up ()
	wl_select_menu_entity ("DAD")
end

function on_key_down ()
   wl_select_menu_entity ("DCX")
end

function on_key_left ()
	wl_select_menu_entity ("ADC")
end

function on_key_right ()
	wl_select_menu_entity ("LXI")
end

function on_key_a ()
   wl_mnemonic_set_base (0x05)
   wl_create_menu ("register", 176, 24, "frame.png")
   wl_create_entity (2, 176, 24, "b", "script/assembler/RS/B.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 32, "c", "script/assembler/RS/C.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 40, "d", "script/assembler/RS/D.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 48, "e", "script/assembler/RS/E.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 56, "h", "script/assembler/RS/H.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 64, "l", "script/assembler/RS/L.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 72, "m", "script/assembler/RS/m.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 176, 80, "A", "script/assembler/RS/A.lua", "white", "red", "yellow", "gray")
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
      "DECREMENT                     " ..
      "                              " ..
      "DCR subtracts one from the    " ..
      "content of the specified byte." ..
      "DCR affects all the condition " ..
      "flags except the carry flag.  " ..
      "Because DCR preserves the     " ..
      "carry flag, it can be used    " ..
      "within a multi-byte arithmetic" ..
      "routine to decrement character" ..
      "counts and similar purposes.  " ..
      "The operand must specify one  " ..
      "of the registers A through E, " ..
      "H or L. It will subtract one  " ..
      "from the contents of the      " ..
      "specified register.           " ..
      "This instruction subtracts one" ..
      "from the contents of the      " ..
      "memory location addressed by  " ..
      "the H and L registers. M is a " ..
      "symbol in reference to the H  " ..
      "and L registers.",
      "frame.png"
   ) 
end

function on_key_start ()

end

function on_key_back ()

end

init ()
