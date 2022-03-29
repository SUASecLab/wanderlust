function init ()

end

function on_key_up ()
   wl_select_menu_entity ("06")
end

function on_key_down ()
   wl_select_menu_entity ("26")
end

function on_key_left ()
   wl_select_menu_entity ("15")
end

function on_key_right ()
   wl_select_menu_entity ("17")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x16)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
