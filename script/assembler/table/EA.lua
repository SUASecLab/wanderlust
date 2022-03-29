function init ()

end

function on_key_up ()
   wl_select_menu_entity ("DA")
end

function on_key_down ()
   wl_select_menu_entity ("FA")
end

function on_key_left ()
   wl_select_menu_entity ("E9")
end

function on_key_right ()
   wl_select_menu_entity ("EB")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xEA)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
