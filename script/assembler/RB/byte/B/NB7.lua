function init ()

end

function on_key_up ()
   wl_select_menu_entity ("6")
end

function on_key_down ()
   wl_select_menu_entity ("8")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_mnemonic_set_source_byte_nb (0x7)
   wl_repeal_menu ()
   wl_repeal_menu ()
   wl_repeal_menu ()
   wl_repeal_menu ()
   wl_mnemonic_apply ()
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
