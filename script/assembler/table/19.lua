function init ()

end

function on_key_up ()
   wl_select_menu_entity ("09")
end

function on_key_down ()
   wl_select_menu_entity ("29")
end

function on_key_left ()
   wl_select_menu_entity ("18")
end

function on_key_right ()
   wl_select_menu_entity ("1A")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x19)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
