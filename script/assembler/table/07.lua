function init ()

end

function on_key_up ()
   wl_select_menu_entity ("F7")
end

function on_key_down ()
   wl_select_menu_entity ("17")
end

function on_key_left ()
   wl_select_menu_entity ("06")
end

function on_key_right ()
   wl_select_menu_entity ("08")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x07)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
