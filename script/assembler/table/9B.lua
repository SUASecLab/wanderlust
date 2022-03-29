function init ()

end

function on_key_up ()
   wl_select_menu_entity ("8B")
end

function on_key_down ()
   wl_select_menu_entity ("AB")
end

function on_key_left ()
   wl_select_menu_entity ("9A")
end

function on_key_right ()
   wl_select_menu_entity ("9C")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x9B)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
