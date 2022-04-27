function init ()

end

function on_key_up ()
	wl_select_menu_entity ("IN")
end

function on_key_down ()
   wl_select_menu_entity ("INX")
end

function on_key_left ()
	wl_select_menu_entity ("CC")
end

function on_key_right ()
	wl_select_menu_entity ("OUT")
end

function on_key_a ()
   wl_mnemonic_set_base (0x04)
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
      "INCREMENT                     " ..
      "                              " ..
      "INR adds one to the content of" ..
      "the specified byte. It affects" ..
      "all of the condition flags ex-" ..
      "cept the carry flag.          " ..
      "Because it preserves the carry" ..
      "flag, it can be used within   " ..
      "multi-byte arithmetic routines" ..
      "for incrementing character    " ..
      "counts and similar purposes.",
      "frame.png"
   )
   wl_create_textbox (
      "white", 
      "INCREMENT REGISTER            " ..
      "                              " ..
      "The operand must specify one  " ..
      "of the registers A through E, " ..
      "H or L. The Instruction adds  " ..
      "one to the contents of the    " ..
      "specified register,",
      "frame.png"
   )
   wl_create_textbox (
      "white", 
      "INCREMENT MEMORY              " ..
      "                              " ..
      "This instruction increments by" ..
      "one the contents of the memory" ..
      "location addressed by the H   " ..
      "and L registers. M is a sym-  " ..
      "bolic reference to the H and L" ..
      "registers.",
      "frame.png"
   )
end

function on_key_start ()
	
end

function on_key_back ()
	
end

init ()
