function init ()

end

function on_key_up ()
   wl_select_menu_entity ("C4")
end

function on_key_down ()
   wl_select_menu_entity ("E4")
end

function on_key_left ()
   wl_select_menu_entity ("D3")
end

function on_key_right ()
   wl_select_menu_entity ("D5")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xD4)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
