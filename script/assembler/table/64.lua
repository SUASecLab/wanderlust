function init ()

end

function on_key_up ()
   wl_select_menu_entity ("54")
end

function on_key_down ()
   wl_select_menu_entity ("74")
end

function on_key_left ()
   wl_select_menu_entity ("63")
end

function on_key_right ()
   wl_select_menu_entity ("65")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x64)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
