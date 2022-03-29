function init ()

end

function on_key_up ()
   wl_select_menu_entity ("Single")
end

function on_key_down ()
	wl_select_menu_entity ("Triple")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
	wl_set_property (0, "WL_SCALING", 2)
	wl_set_scaling_factor (2)
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
