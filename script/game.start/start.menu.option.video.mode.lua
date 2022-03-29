function init ()

end

function on_key_up ()
   wl_select_menu_entity ("Scaling")
end

function on_key_down ()

end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_create_menu ("resolution", 224, 152, "frame.png")
   
   wl_create_entity (2, 224,152, "Fullscreen", "script/game.start/start.menu.option.video.mode.fullscreen.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224,168, "Window", "script/game.start/start.menu.option.video.mode.window.lua", "white", "red", "yellow", "gray")
   
   wl_append_menu ()
   
   wl_rundown_task ()

	 a = wl_get_property ("WL_FULLSCREEN")

	if a == nil then
		wl_set_property (0, "WL_FULLSCREEN", 0)
  	wl_select_menu_entity ("Window")
  end
  
  if a == 0 then
    	wl_select_menu_entity ("Window")
  end
  
  if a == 1 then
  	wl_select_menu_entity ("Fullscreen")
  end
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
