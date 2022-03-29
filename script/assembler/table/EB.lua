function init ()

end

function on_key_up ()
   wl_select_menu_entity ("DB")
end

function on_key_down ()
   wl_select_menu_entity ("FB")
end

function on_key_left ()
   wl_select_menu_entity ("EA")
end

function on_key_right ()
   wl_select_menu_entity ("EC")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xEB)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
