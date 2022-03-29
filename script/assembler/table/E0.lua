function init ()

end

function on_key_up ()
   wl_select_menu_entity ("D0")
end

function on_key_down ()
   wl_select_menu_entity ("F0")
end

function on_key_left ()
   wl_select_menu_entity ("EF")
end

function on_key_right ()
   wl_select_menu_entity ("E1")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xE0)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
