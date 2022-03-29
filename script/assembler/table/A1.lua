function init ()

end

function on_key_up ()
   wl_select_menu_entity ("91")
end

function on_key_down ()
   wl_select_menu_entity ("B1")
end

function on_key_left ()
   wl_select_menu_entity ("A0")
end

function on_key_right ()
   wl_select_menu_entity ("A2")
end

function on_key_a ()
   wl_mnemonic_write_byte (0xA1)
end

function on_key_b ()
      wl_repeal_menu ()
end

init ()
