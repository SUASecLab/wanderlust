function init ()

end

function on_key_up ()
   wl_select_menu_entity ("53")
end

function on_key_down ()
   wl_select_menu_entity ("73")
end

function on_key_left ()
   wl_select_menu_entity ("62")
end

function on_key_right ()
   wl_select_menu_entity ("64")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x63)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
