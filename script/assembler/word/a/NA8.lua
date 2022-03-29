function init ()

end

function on_key_up ()
   wl_select_menu_entity ("7")
end

function on_key_down ()
   wl_select_menu_entity ("9")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_mnemonic_set_source_word_na (0x8)
   wl_create_menu ("byte_b", 200, 24, "frame.png")
   wl_create_entity (2, 200, 24, "0", "script/assembler/word/b/NB0.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 32, "1", "script/assembler/word/b/NB1.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 40, "2", "script/assembler/word/b/NB2.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 48, "3", "script/assembler/word/b/NB3.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 56, "4", "script/assembler/word/b/NB4.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 64, "5", "script/assembler/word/b/NB5.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 72, "6", "script/assembler/word/b/NB6.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 80, "7", "script/assembler/word/b/NB7.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 88, "8", "script/assembler/word/b/NB8.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 96, "9", "script/assembler/word/b/NB9.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 104, "A", "script/assembler/word/b/NBA.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 112, "B", "script/assembler/word/b/NBB.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 120, "C", "script/assembler/word/b/NBC.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 128, "D", "script/assembler/word/b/NBD.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 136, "E", "script/assembler/word/b/NBE.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 144, "F", "script/assembler/word/b/NBF.lua", "white", "red", "yellow", "gray")
   wl_append_menu ()
   wl_rundown_task ()
   wl_select_menu_entity ("0")
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
