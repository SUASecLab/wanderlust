function init ()

end

function on_key_up ()
   wl_select_menu_entity ("65")
end

function on_key_down ()
   wl_select_menu_entity ("85")
end

function on_key_left ()
   wl_select_menu_entity ("74")
end

function on_key_right ()
   wl_select_menu_entity ("76")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x75)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
