function init ()

end

function on_key_up ()
   wl_select_menu_entity ("12")
end

function on_key_down ()
   wl_select_menu_entity ("32")
end

function on_key_left ()
   wl_select_menu_entity ("21")
end

function on_key_right ()
   wl_select_menu_entity ("23")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x22)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
