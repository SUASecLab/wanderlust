function init ()

end

function on_key_up ()
   wl_select_menu_entity ("27")
end

function on_key_down ()
   wl_select_menu_entity ("47")
end

function on_key_left ()
   wl_select_menu_entity ("36")
end

function on_key_right ()
   wl_select_menu_entity ("38")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x37)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
