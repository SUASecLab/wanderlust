function init ()

end

function on_key_up ()
   wl_select_menu_entity ("9F")
end

function on_key_down ()
   wl_select_menu_entity ("BF")
end

function on_key_left ()
   wl_select_menu_entity ("AE")
end

function on_key_right ()
   wl_select_menu_entity ("A0")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xAF)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
