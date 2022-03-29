function init ()

end

function on_key_up ()
   wl_select_menu_entity ("92")
end

function on_key_down ()
   wl_select_menu_entity ("B2")
end

function on_key_left ()
   wl_select_menu_entity ("A1")
end

function on_key_right ()
   wl_select_menu_entity ("A3")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xA2)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
