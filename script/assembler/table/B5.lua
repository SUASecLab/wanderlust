function init ()

end

function on_key_up ()
   wl_select_menu_entity ("A5")
end

function on_key_down ()
   wl_select_menu_entity ("C5")
end

function on_key_left ()
   wl_select_menu_entity ("B4")
end

function on_key_right ()
   wl_select_menu_entity ("B6")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xB5)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
