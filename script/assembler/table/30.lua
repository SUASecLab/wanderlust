function init ()

end

function on_key_up ()
   wl_select_menu_entity ("20")
end

function on_key_down ()
   wl_select_menu_entity ("40")
end

function on_key_left ()
   wl_select_menu_entity ("3F")
end

function on_key_right ()
   wl_select_menu_entity ("31")
end

function on_key_a ()
   wl_mnemonic_write_byte (0x30)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
