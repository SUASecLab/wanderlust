function init ()

end

function on_key_up ()
   wl_select_menu_entity ("AF")
end

function on_key_down ()
   wl_select_menu_entity ("CF")
end

function on_key_left ()
   wl_select_menu_entity ("BE")
end

function on_key_right ()
   wl_select_menu_entity ("B0")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xBF)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
