function init ()

end

function on_key_up ()
   wl_select_menu_entity ("F")
end

function on_key_down ()
   wl_select_menu_entity ("1")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_mnemonic_set_source_byte_na (0x0)
   wl_create_menu ("byte_b", 200, 8, "frame.png")
   wl_create_entity (2, 200, 8, "0", "script/assembler/byte/NB0.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 16, "1", "script/assembler/byte/NB1.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 24, "2", "script/assembler/byte/NB2.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 32, "3", "script/assembler/byte/NB3.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 40, "4", "script/assembler/byte/NB4.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 48, "5", "script/assembler/byte/NB5.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 56, "6", "script/assembler/byte/NB6.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 64, "7", "script/assembler/byte/NB7.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 72, "8", "script/assembler/byte/NB8.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 80, "9", "script/assembler/byte/NB9.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 88, "A", "script/assembler/byte/NBA.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 96, "B", "script/assembler/byte/NBB.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 104, "C", "script/assembler/byte/NBC.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 112, "D", "script/assembler/byte/NBD.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 120, "E", "script/assembler/byte/NBE.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 200, 128, "F", "script/assembler/byte/NBF.lua", "white", "red", "yellow", "gray")
   wl_append_menu ()
   wl_rundown_task ()
   wl_select_menu_entity ("0")
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
