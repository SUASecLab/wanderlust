function init ()

end

function on_key_up ()
   wl_select_menu_entity ("HL")
end

function on_key_down ()
   wl_select_menu_entity ("BC")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_mnemonic_set_target_register_pair (0x3)
   wl_repeal_menu ()
   wl_repeal_menu ()
   wl_mnemonic_apply ()   
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
