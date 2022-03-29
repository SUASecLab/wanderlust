function init ()

end

function on_key_up ()
   wl_select_menu_entity ("8C")
end

function on_key_down ()
   wl_select_menu_entity ("AC")
end

function on_key_left ()
   wl_select_menu_entity ("9B")
end

function on_key_right ()
   wl_select_menu_entity ("9D")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x9C)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
