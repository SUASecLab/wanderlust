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
   wl_create_menu ("byte_c", 256, 24, "frame.png")
   wl_create_entity (2, 256,  24, "0", "script/assembler/PW/word/c/NC0.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 256,  32, "1", "script/assembler/PW/word/c/NC1.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 256,  40, "2", "script/assembler/PW/word/c/NC2.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 256,  48, "3", "script/assembler/PW/word/c/NC3.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 256,  56, "4", "script/assembler/PW/word/c/NC4.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 256,  64, "5", "script/assembler/PW/word/c/NC5.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 256,  72, "6", "script/assembler/PW/word/c/NC6.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 256,  80, "7", "script/assembler/PW/word/c/NC7.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 256,  88, "8", "script/assembler/PW/word/c/NC8.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 256,  96, "9", "script/assembler/PW/word/c/NC9.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 256, 104, "A", "script/assembler/PW/word/c/NCA.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 256, 112, "B", "script/assembler/PW/word/c/NCB.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 256, 120, "C", "script/assembler/PW/word/c/NCC.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 256, 128, "D", "script/assembler/PW/word/c/NCD.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 256, 136, "E", "script/assembler/PW/word/c/NCE.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 256, 144, "F", "script/assembler/PW/word/c/NCF.lua", "white", "red", "yellow", "gray")
   wl_append_menu ()
   wl_rundown_task ()
   wl_select_menu_entity ("0")
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
