function init ()

end

function on_key_up ()
   wl_select_menu_entity ("GOTO")
end

function on_key_down ()
   wl_select_menu_entity ("RPE")
end

function on_key_left ()
   wl_select_menu_entity ("RP")
end

function on_key_right ()
   wl_select_menu_entity ("DAA")
end

function on_key_a ()
   wl_repeal_menu ()
end

function on_key_b ()

end

init ()
