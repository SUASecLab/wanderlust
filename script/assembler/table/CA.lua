function init ()

end

function on_key_up ()
   wl_select_menu_entity ("BA")
end

function on_key_down ()
   wl_select_menu_entity ("DA")
end

function on_key_left ()
   wl_select_menu_entity ("C9")
end

function on_key_right ()
   wl_select_menu_entity ("CB")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xCA)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
