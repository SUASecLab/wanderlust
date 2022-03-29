function init ()

end

function on_key_up ()
   wl_select_menu_entity ("2")
end

function on_key_down ()
   wl_select_menu_entity ("4")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_mnemonic_set_source_word_nd (0x3)
   wl_repeal_menu ()
   wl_repeal_menu ()
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
