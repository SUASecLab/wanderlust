function init ()

end

function on_key_up ()
   wl_select_menu_entity ("D")
end

function on_key_down ()
   wl_select_menu_entity ("F")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_mnemonic_set_source_word_nc (0xE)
   wl_create_menu ("byte_c", 280, 24, "frame.png")
   wl_create_entity (2, 280,  24, "0", "script/assembler/PW/word/d/ND0.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 280,  32, "1", "script/assembler/PW/word/d/ND1.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 280,  40, "2", "script/assembler/PW/word/d/ND2.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 280,  48, "3", "script/assembler/PW/word/d/ND3.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 280,  56, "4", "script/assembler/PW/word/d/ND4.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 280,  64, "5", "script/assembler/PW/word/d/ND5.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 280,  72, "6", "script/assembler/PW/word/d/ND6.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 280,  80, "7", "script/assembler/PW/word/d/ND7.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 280,  88, "8", "script/assembler/PW/word/d/ND8.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 280,  96, "9", "script/assembler/PW/word/d/ND9.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 280, 104, "A", "script/assembler/PW/word/d/NDA.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 280, 112, "B", "script/assembler/PW/word/d/NDB.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 280, 120, "C", "script/assembler/PW/word/d/NDC.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 280, 128, "D", "script/assembler/PW/word/d/NDD.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 280, 136, "E", "script/assembler/PW/word/d/NDE.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 280, 144, "F", "script/assembler/PW/word/d/NDF.lua", "white", "red", "yellow", "gray")
   wl_append_menu ()
   wl_rundown_task ()
   wl_select_menu_entity ("0")
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
