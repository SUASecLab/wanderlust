function init ()

end

function on_key_up ()
   wl_select_menu_entity ("4D")
end

function on_key_down ()
   wl_select_menu_entity ("6D")
end

function on_key_left ()
   wl_select_menu_entity ("5C")
end

function on_key_right ()
   wl_select_menu_entity ("5E")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x5D)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
