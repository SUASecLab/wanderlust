function init ()

end

function on_key_up ()
   wl_select_menu_entity ("E")
end

function on_key_down ()
   wl_select_menu_entity ("0")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_mnemonic_set_source_word_nc (0xF)
   wl_create_menu ("byte_d", 248, 24, "frame.png")
   wl_create_entity (2, 248, 24, "0", "script/assembler/word/d/ND0.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 248, 32, "1", "script/assembler/word/d/ND1.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 248, 40, "2", "script/assembler/word/d/ND2.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 248, 48, "3", "script/assembler/word/d/ND3.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 248, 56, "4", "script/assembler/word/d/ND4.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 248, 64, "5", "script/assembler/word/d/ND5.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 248, 72, "6", "script/assembler/word/d/ND6.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 248, 80, "7", "script/assembler/word/d/ND7.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 248, 88, "8", "script/assembler/word/d/ND8.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 248, 96, "9", "script/assembler/word/d/ND9.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 248, 104, "A", "script/assembler/word/d/NDA.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 248, 112, "B", "script/assembler/word/d/NDB.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 248, 120, "C", "script/assembler/word/d/NDC.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 248, 128, "D", "script/assembler/word/d/NDD.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 248, 136, "E", "script/assembler/word/d/NDE.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 248, 144, "F", "script/assembler/word/d/NDF.lua", "white", "red", "yellow", "gray")
   wl_append_menu ()
   wl_rundown_task ()
   wl_select_menu_entity ("0")
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
