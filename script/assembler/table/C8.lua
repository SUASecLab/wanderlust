function init ()

end

function on_key_up ()
   wl_select_menu_entity ("B8")
end

function on_key_down ()
   wl_select_menu_entity ("D8")
end

function on_key_left ()
   wl_select_menu_entity ("C7")
end

function on_key_right ()
   wl_select_menu_entity ("C9")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xC8)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
