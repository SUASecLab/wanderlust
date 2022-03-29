function init ()

end

function on_key_up ()
   wl_select_menu_entity ("AC")
end

function on_key_down ()
   wl_select_menu_entity ("CC")
end

function on_key_left ()
   wl_select_menu_entity ("BB")
end

function on_key_right ()
   wl_select_menu_entity ("BD")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xBC)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
