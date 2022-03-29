function init ()

end

function on_key_up ()
   wl_select_menu_entity ("CA")
end

function on_key_down ()
   wl_select_menu_entity ("EA")
end

function on_key_left ()
   wl_select_menu_entity ("D9")
end

function on_key_right ()
   wl_select_menu_entity ("DB")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xDA)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
