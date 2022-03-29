function init ()

end

function on_key_up ()
   wl_select_menu_entity ("61")
end

function on_key_down ()
   wl_select_menu_entity ("81")
end

function on_key_left ()
   wl_select_menu_entity ("70")
end

function on_key_right ()
   wl_select_menu_entity ("72")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x71)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
