function init ()

end

function on_key_up ()
   wl_select_menu_entity ("CD")
end

function on_key_down ()
   wl_select_menu_entity ("ED")
end

function on_key_left ()
   wl_select_menu_entity ("DC")
end

function on_key_right ()
   wl_select_menu_entity ("DE")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xDD)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
