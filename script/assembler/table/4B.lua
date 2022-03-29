function init ()

end

function on_key_up ()
   wl_select_menu_entity ("3B")
end

function on_key_down ()
   wl_select_menu_entity ("5B")
end

function on_key_left ()
   wl_select_menu_entity ("4A")
end

function on_key_right ()
   wl_select_menu_entity ("4C")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x4B)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
