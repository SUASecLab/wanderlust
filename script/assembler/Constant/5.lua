function init ()

end

function on_key_up ()
   wl_select_menu_entity ("4")
end

function on_key_down ()
   wl_select_menu_entity ("6")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_mnemonic_set_target_constant (0x5)
   wl_repeal_menu ()
   wl_repeal_menu ()
   wl_mnemonic_apply ()
end

function on_key_b ()

end

init ()
