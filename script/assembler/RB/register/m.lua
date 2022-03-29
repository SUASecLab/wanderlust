function init ()

end

function on_key_up ()
   wl_select_menu_entity ("l")
end

function on_key_down ()
   wl_select_menu_entity ("A")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_mnemonic_set_target_register (0x6)
   wl_create_menu ("byte_a", 200, 8, "frame.png")
   wl_create_entity (2, 200,   8, "0", "script/assembler/RB/byte/A/NA0.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200,  16, "1", "script/assembler/RB/byte/A/NA1.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200,  24, "2", "script/assembler/RB/byte/A/NA2.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200,  32, "3", "script/assembler/RB/byte/A/NA3.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200,  40, "4", "script/assembler/RB/byte/A/NA4.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200,  48, "5", "script/assembler/RB/byte/A/NA5.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200,  56, "6", "script/assembler/RB/byte/A/NA6.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200,  64, "7", "script/assembler/RB/byte/A/NA7.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200,  72, "8", "script/assembler/RB/byte/A/NA8.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200,  80, "9", "script/assembler/RB/byte/A/NA9.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200,  88, "A", "script/assembler/RB/byte/A/NAA.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200,  96, "B", "script/assembler/RB/byte/A/NAB.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 104, "C", "script/assembler/RB/byte/A/NAC.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 112, "D", "script/assembler/RB/byte/A/NAD.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 120, "E", "script/assembler/RB/byte/A/NAE.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 128, "F", "script/assembler/RB/byte/A/NAF.lua", "white", "red", "yellow", "gray")
   wl_append_menu ()
   wl_rundown_task ()
   wl_select_menu_entity ("0")
end

function on_key_b ()

end

init ()
