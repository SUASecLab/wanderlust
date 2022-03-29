function init ()

end

function on_key_up ()
   wl_select_menu_entity ("Music")
end

function on_key_down ()
   wl_select_menu_entity ("Silent")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_create_menu ("effects", 224, 136, "frame.png")
   
   wl_create_entity (2, 224,136, "On", "script/game.start/start.menu.option.audio.effects.on.lua", "white", "red", "yellow", "gray")

   wl_create_entity (2, 224,152, "Off", "script/game.start/start.menu.option.audio.effects.off.lua", "white", "red", "yellow", "gray")
   
   wl_append_menu ()
   
   wl_rundown_task ()
   
   wl_select_menu_entity ("On")
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
