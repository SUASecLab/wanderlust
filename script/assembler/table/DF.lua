function init ()

end

function on_key_up ()
   wl_select_menu_entity ("CF")
end

function on_key_down ()
   wl_select_menu_entity ("EF")
end

function on_key_left ()
   wl_select_menu_entity ("DE")
end

function on_key_right ()
   wl_select_menu_entity ("D0")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xDF)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
