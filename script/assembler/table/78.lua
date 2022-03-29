function init ()

end

function on_key_up ()
   wl_select_menu_entity ("68")
end

function on_key_down ()
   wl_select_menu_entity ("88")
end

function on_key_left ()
   wl_select_menu_entity ("77")
end

function on_key_right ()
   wl_select_menu_entity ("79")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x78)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
