function init ()

end

function on_key_up ()
   wl_select_menu_entity ("B5")
end

function on_key_down ()
   wl_select_menu_entity ("D5")
end

function on_key_left ()
   wl_select_menu_entity ("C4")
end

function on_key_right ()
   wl_select_menu_entity ("C6")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xC5)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
