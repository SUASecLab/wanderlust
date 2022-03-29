function init ()

end

function on_key_up ()
   wl_select_menu_entity ("DE")
end

function on_key_down ()
   wl_select_menu_entity ("BC")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_mnemonic_set_target_register_pair (0x02);
   wl_create_menu ("byte_a", 208, 24, "frame.png")
   wl_create_entity (2, 208,  24, "0", "script/assembler/PW/word/a/NA0.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 208,  32, "1", "script/assembler/PW/word/a/NA1.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 208,  40, "2", "script/assembler/PW/word/a/NA2.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 208,  48, "3", "script/assembler/PW/word/a/NA3.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 208,  56, "4", "script/assembler/PW/word/a/NA4.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 208,  64, "5", "script/assembler/PW/word/a/NA5.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 208,  72, "6", "script/assembler/PW/word/a/NA6.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 208,  80, "7", "script/assembler/PW/word/a/NA7.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 208,  88, "8", "script/assembler/PW/word/a/NA8.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 208,  96, "9", "script/assembler/PW/word/a/NA9.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 208, 104, "A", "script/assembler/PW/word/a/NAA.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 208, 112, "B", "script/assembler/PW/word/a/NAB.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 208, 120, "C", "script/assembler/PW/word/a/NAC.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 208, 128, "D", "script/assembler/PW/word/a/NAD.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 208, 136, "E", "script/assembler/PW/word/a/NAE.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 208, 144, "F", "script/assembler/PW/word/a/NAF.lua", "white", "red", "yellow", "gray")
   wl_append_menu ()
   wl_rundown_task ()
   wl_select_menu_entity ("0")
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
