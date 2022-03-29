function init ()

end

function on_key_up ()
   wl_select_menu_entity ("Audio")
end

function on_key_down ()
   wl_select_menu_entity ("Story")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_create_menu ("video", 152, 136, "frame.png")
   
   wl_create_entity (2, 152,136, "Scaling", "script/game.start/start.menu.option.video.scaling.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 152,152, "Mode", "script/game.start/start.menu.option.video.mode.lua", "white", "red", "yellow", "gray")

   wl_append_menu ()
   
   wl_rundown_task ()
   
   wl_select_menu_entity ("Scaling")
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
