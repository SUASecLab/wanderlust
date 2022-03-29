function init ()

end

function on_key_up ()
   wl_select_menu_entity ("9E")
end

function on_key_down ()
   wl_select_menu_entity ("BE")
end

function on_key_left ()
   wl_select_menu_entity ("AD")
end

function on_key_right ()
   wl_select_menu_entity ("AF")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xAE)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
