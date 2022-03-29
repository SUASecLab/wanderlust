function init ()

end

function on_key_up ()
   wl_select_menu_entity ("6F")
end

function on_key_down ()
   wl_select_menu_entity ("8F")
end

function on_key_left ()
   wl_select_menu_entity ("7E")
end

function on_key_right ()
   wl_select_menu_entity ("70")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x7F)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
