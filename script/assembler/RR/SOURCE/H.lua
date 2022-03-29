function init ()

end

function on_key_up ()
   wl_select_menu_entity ("e")
end

function on_key_down ()
   wl_select_menu_entity ("l")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_mnemonic_set_source_register (0x4)
   wl_repeal_menu ()
   wl_repeal_menu ()
   wl_repeal_menu ()
   wl_mnemonic_apply ()
end

function on_key_b ()

end

init ()
