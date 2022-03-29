function init ()

end

function on_key_up ()
   wl_select_menu_entity ("c")
end

function on_key_down ()
   wl_select_menu_entity ("e")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_mnemonic_set_target_register (0x2)
   wl_create_menu ("register_b", 200, 24, "frame.png")
   wl_create_entity (2, 200, 24, "b", "script/assembler/RR/SOURCE/B.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 32, "c", "script/assembler/RR/SOURCE/C.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 40, "d", "script/assembler/RR/SOURCE/D.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 48, "e", "script/assembler/RR/SOURCE/E.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 56, "h", "script/assembler/RR/SOURCE/H.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 64, "l", "script/assembler/RR/SOURCE/L.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 72, "m", "script/assembler/RR/SOURCE/m.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 80, "A", "script/assembler/RR/SOURCE/A.lua", "white", "red", "yellow", "gray")
   wl_append_menu ()
   wl_rundown_task ()
   wl_select_menu_entity ("b")
end

function on_key_b ()

end

init ()
