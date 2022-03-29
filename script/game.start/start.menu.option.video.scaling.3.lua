function init ()

end

function on_key_up ()
	wl_select_menu_entity ("Double")
end

function on_key_down ()
  wl_select_menu_entity ("Quad")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
	wl_set_property (0, "WL_SCALING", 3)
	wl_set_scaling_factor (3)
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
