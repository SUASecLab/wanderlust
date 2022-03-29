function init ()

end

function on_key_up ()
   wl_select_menu_entity ("DF")
end

function on_key_down ()
   wl_select_menu_entity ("FF")
end

function on_key_left ()
   wl_select_menu_entity ("EE")
end

function on_key_right ()
   wl_select_menu_entity ("E0")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xEF)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
