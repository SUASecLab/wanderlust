function init ()

end

function on_key_up ()
   wl_select_menu_entity ("52")
end

function on_key_down ()
   wl_select_menu_entity ("72")
end

function on_key_left ()
   wl_select_menu_entity ("61")
end

function on_key_right ()
   wl_select_menu_entity ("63")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x62)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
