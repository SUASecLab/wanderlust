function init ()

end

function on_key_up ()
   wl_select_menu_entity ("EB")
end

function on_key_down ()
   wl_select_menu_entity ("0B")
end

function on_key_left ()
   wl_select_menu_entity ("FA")
end

function on_key_right ()
   wl_select_menu_entity ("FC")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xFB)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
