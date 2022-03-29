function init ()

end

function on_key_up ()
   wl_select_menu_entity ("94")
end

function on_key_down ()
   wl_select_menu_entity ("B4")
end

function on_key_left ()
   wl_select_menu_entity ("A3")
end

function on_key_right ()
   wl_select_menu_entity ("A5")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xA4)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
