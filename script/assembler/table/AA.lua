function init ()

end

function on_key_up ()
   wl_select_menu_entity ("9A")
end

function on_key_down ()
   wl_select_menu_entity ("BA")
end

function on_key_left ()
   wl_select_menu_entity ("A9")
end

function on_key_right ()
   wl_select_menu_entity ("AB")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xAA)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
