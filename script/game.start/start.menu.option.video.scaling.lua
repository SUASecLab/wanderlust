function init ()

end

function on_key_up ()

end

function on_key_down ()
   wl_select_menu_entity ("Mode")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_create_menu ("scaling", 224, 136, "frame.png")
   
--   wl_create_entity (2, 224,136, "decrement", "script/game.start/start.menu.option.video.scaling.decrement.lua", "white", "red", "yellow", "gray")
--   wl_create_entity (2, 224,144, "increment", "script/game.start/start.menu.option.video.scaling.increment.lua", "white", "red", "yellow", "gray")
   

   wl_create_entity (2, 224,136, "Single", "script/game.start/start.menu.option.video.scaling.1.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224,144, "Double", "script/game.start/start.menu.option.video.scaling.2.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224,152, "Triple", "script/game.start/start.menu.option.video.scaling.3.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 224,160, "Quad", "script/game.start/start.menu.option.video.scaling.4.lua", "white", "red", "yellow", "gray")
   
   wl_append_menu ()
   
   wl_rundown_task ()

	wl_select_menu_entity ("decrement")

	 a = wl_get_property ("WL_SCALING")
	 
	 if a == null then
	   wl_select_menu_entity ("Single")
	 end
	 
	 if a == 1 then
	    wl_select_menu_entity ("Single")
	 end
	 
	 if a == 2 then
	    wl_select_menu_entity ("Double")
	 end
	 
	 if a == 3 then
	    wl_select_menu_entity ("Triple")
	 end
	 
	 if a == 4 then
	    wl_select_menu_entity ("Quad")
	 end
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
