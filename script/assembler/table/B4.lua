function init ()

end

function on_key_up ()
   wl_select_menu_entity ("A4")
end

function on_key_down ()
   wl_select_menu_entity ("C4")
end

function on_key_left ()
   wl_select_menu_entity ("B3")
end

function on_key_right ()
   wl_select_menu_entity ("B5")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xB4)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
