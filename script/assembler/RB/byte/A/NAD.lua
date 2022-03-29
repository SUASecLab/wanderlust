function init ()

end

function on_key_up ()
   wl_select_menu_entity ("C")
end

function on_key_down ()
   wl_select_menu_entity ("E")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_mnemonic_set_source_byte_na (0xD)
   wl_create_menu ("byte_a", 224, 8, "frame.png")
   wl_create_entity (2, 224,   8, "0", "script/assembler/RB/byte/B/NB0.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224,  16, "1", "script/assembler/RB/byte/B/NB1.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224,  24, "2", "script/assembler/RB/byte/B/NB2.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224,  32, "3", "script/assembler/RB/byte/B/NB3.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224,  40, "4", "script/assembler/RB/byte/B/NB4.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224,  48, "5", "script/assembler/RB/byte/B/NB5.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224,  56, "6", "script/assembler/RB/byte/B/NB6.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224,  64, "7", "script/assembler/RB/byte/B/NB7.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224,  72, "8", "script/assembler/RB/byte/B/NB8.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224,  80, "9", "script/assembler/RB/byte/B/NB9.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224,  88, "A", "script/assembler/RB/byte/B/NBA.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224,  96, "B", "script/assembler/RB/byte/B/NBB.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 104, "C", "script/assembler/RB/byte/B/NBC.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 112, "D", "script/assembler/RB/byte/B/NBD.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 120, "E", "script/assembler/RB/byte/B/NBE.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224, 128, "F", "script/assembler/RB/byte/B/NBF.lua", "white", "red", "yellow", "gray")
   wl_append_menu ()
   wl_rundown_task ()
   wl_select_menu_entity ("0")
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
