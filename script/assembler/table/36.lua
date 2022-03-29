function init ()

end

function on_key_up ()
   wl_select_menu_entity ("26")
end

function on_key_down ()
   wl_select_menu_entity ("46")
end

function on_key_left ()
   wl_select_menu_entity ("35")
end

function on_key_right ()
   wl_select_menu_entity ("37")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x36)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
