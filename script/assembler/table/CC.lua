function init ()

end

function on_key_up ()
   wl_select_menu_entity ("BC")
end

function on_key_down ()
   wl_select_menu_entity ("DC")
end

function on_key_left ()
   wl_select_menu_entity ("CB")
end

function on_key_right ()
   wl_select_menu_entity ("CD")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xCC)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
