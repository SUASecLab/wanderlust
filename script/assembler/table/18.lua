function init ()

end

function on_key_up ()
   wl_select_menu_entity ("08")
end

function on_key_down ()
   wl_select_menu_entity ("28")
end

function on_key_left ()
   wl_select_menu_entity ("17")
end

function on_key_right ()
   wl_select_menu_entity ("19")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x18)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
