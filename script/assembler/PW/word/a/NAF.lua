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
   wl_mnemonic_set_source_word_na (0xF)
   wl_create_menu ("byte_b", 232,  24, "frame.png")
   wl_create_entity (2, 232,  24, "0", "script/assembler/PW/word/b/NB0.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 232,  32, "1", "script/assembler/PW/word/b/NB1.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 232,  40, "2", "script/assembler/PW/word/b/NB2.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 232,  48, "3", "script/assembler/PW/word/b/NB3.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 232,  56, "4", "script/assembler/PW/word/b/NB4.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 232,  64, "5", "script/assembler/PW/word/b/NB5.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 232,  72, "6", "script/assembler/PW/word/b/NB6.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 232,  80, "7", "script/assembler/PW/word/b/NB7.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 232,  88, "8", "script/assembler/PW/word/b/NB8.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 232,  96, "9", "script/assembler/PW/word/b/NB9.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 232, 104, "A", "script/assembler/PW/word/b/NBA.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 232, 112, "B", "script/assembler/PW/word/b/NBB.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 232, 120, "C", "script/assembler/PW/word/b/NBC.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 232, 128, "D", "script/assembler/PW/word/b/NBD.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 232, 136, "E", "script/assembler/PW/word/b/NBE.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 232, 144, "F", "script/assembler/PW/word/b/NBF.lua", "white", "red", "yellow", "gray")
   wl_append_menu ()
   wl_rundown_task ()
   wl_select_menu_entity ("0")
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
