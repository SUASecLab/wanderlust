function init ()

end

function on_key_up ()
	wl_select_menu_entity ("LDA")
end

function on_key_down ()
   wl_select_menu_entity ("DAD")
end

function on_key_left ()
	wl_select_menu_entity ("DAA")
end

function on_key_right ()
	wl_select_menu_entity ("RP")
end

function on_key_a ()
	wl_mnemonic_set_base (0x0A)
	wl_create_menu ("byte_a", 176, 24, "frame.png")
	wl_create_entity (2, 176, 24, "BC", "script/assembler/RP/BC_.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 32, "DE", "script/assembler/RP/DE_.lua", "white", "red", "yellow", "gray")
	wl_append_menu ()
	wl_rundown_task ()
	wl_select_menu_entity ("BC")
end

function on_key_b ()
	wl_repeal_menu ()
end

function on_key_x ()

end

function on_key_y ()
	wl_create_textbox (
      "white", 
      "LOAD ACCUMULATOR INDIRECT     " ..
      "                              " ..
      "LDAX loads the accumulator    " ..
      "with a copy of the byte stored" ..
      "at the memory location addres-" ..
      "sed by register pair B or D.  " ..
      "The operand B specifies the B " ..
      "and C register pair and vice  " ..
      "versa D the pair of D and E.  " ..
      "This instruction may specify  " ..
      "only the B or D register pair.",
      "frame.png"
   )
end

function on_key_start ()

end

function on_key_back ()

end

init ()
