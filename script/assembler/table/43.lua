function init ()

end

function on_key_up ()
   wl_select_menu_entity ("33")
end

function on_key_down ()
   wl_select_menu_entity ("53")
end

function on_key_left ()
   wl_select_menu_entity ("42")
end

function on_key_right ()
   wl_select_menu_entity ("44")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x43)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
