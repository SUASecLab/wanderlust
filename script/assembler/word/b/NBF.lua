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
   wl_mnemonic_set_source_word_nb (0xF)
   wl_create_menu ("byte_c", 224, 24, "frame.png")
   wl_create_entity (2, 224, 24, "0", "script/assembler/word/c/NC0.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 32, "1", "script/assembler/word/c/NC1.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 40, "2", "script/assembler/word/c/NC2.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 48, "3", "script/assembler/word/c/NC3.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 56, "4", "script/assembler/word/c/NC4.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 64, "5", "script/assembler/word/c/NC5.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 72, "6", "script/assembler/word/c/NC6.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 80, "7", "script/assembler/word/c/NC7.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 88, "8", "script/assembler/word/c/NC8.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 96, "9", "script/assembler/word/c/NC9.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 104, "A", "script/assembler/word/c/NCA.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 112, "B", "script/assembler/word/c/NCB.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 120, "C", "script/assembler/word/c/NCC.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 128, "D", "script/assembler/word/c/NCD.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 136, "E", "script/assembler/word/c/NCE.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 144, "F", "script/assembler/word/c/NCF.lua", "white", "red", "yellow", "gray")
   wl_append_menu ()
   wl_rundown_task ()
   wl_select_menu_entity ("0")
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
