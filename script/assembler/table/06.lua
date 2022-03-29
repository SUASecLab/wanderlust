function init ()

end

function on_key_up ()
   wl_select_menu_entity ("F6")
end

function on_key_down ()
   wl_select_menu_entity ("16")
end

function on_key_left ()
   wl_select_menu_entity ("05")
end

function on_key_right ()
   wl_select_menu_entity ("07")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x06)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
