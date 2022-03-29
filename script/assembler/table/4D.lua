function init ()

end

function on_key_up ()
   wl_select_menu_entity ("3D")
end

function on_key_down ()
   wl_select_menu_entity ("5D")
end

function on_key_left ()
   wl_select_menu_entity ("4C")
end

function on_key_right ()
   wl_select_menu_entity ("4E")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x4D)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
