function init ()

end

function on_key_up ()
   wl_select_menu_entity ("E3")
end

function on_key_down ()
   wl_select_menu_entity ("03")
end

function on_key_left ()
   wl_select_menu_entity ("F2")
end

function on_key_right ()
   wl_select_menu_entity ("F4")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xF3)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
