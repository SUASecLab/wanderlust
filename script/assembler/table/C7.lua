function init ()

end

function on_key_up ()
   wl_select_menu_entity ("B7")
end

function on_key_down ()
   wl_select_menu_entity ("D7")
end

function on_key_left ()
   wl_select_menu_entity ("C6")
end

function on_key_right ()
   wl_select_menu_entity ("C8")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xC7)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
