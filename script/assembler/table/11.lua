function init ()

end

function on_key_up ()
   wl_select_menu_entity ("01")
end

function on_key_down ()
   wl_select_menu_entity ("21")
end

function on_key_left ()
   wl_select_menu_entity ("10")
end

function on_key_right ()
   wl_select_menu_entity ("12")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x11)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
