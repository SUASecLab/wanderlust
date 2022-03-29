function init ()

end

function on_key_up ()
   wl_select_menu_entity ("7F")
end

function on_key_down ()
   wl_select_menu_entity ("9F")
end

function on_key_left ()
   wl_select_menu_entity ("8E")
end

function on_key_right ()
   wl_select_menu_entity ("80")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x8F)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
