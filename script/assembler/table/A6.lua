function init ()

end

function on_key_up ()
   wl_select_menu_entity ("96")
end

function on_key_down ()
   wl_select_menu_entity ("B6")
end

function on_key_left ()
   wl_select_menu_entity ("A5")
end

function on_key_right ()
   wl_select_menu_entity ("A7")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xA6)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
