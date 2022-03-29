function init ()

end

function on_key_up ()
   wl_select_menu_entity ("Input")
end

function on_key_down ()
   wl_select_menu_entity ("Video")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_create_menu ("audio", 152, 120, "frame.png")
   
   wl_create_entity (2, 152,120, "Music", "script/game.start/start.menu.option.audio.music.lua", "white", "red", "yellow", "gray")
   
   wl_create_entity (2, 152,136, "Effects", "script/game.start/start.menu.option.audio.effects.lua", "white", "red", "yellow", "gray")

   wl_create_entity (2, 152,152, "Silent", "script/game.start/start.menu.option.audio.silent.lua", "white", "red", "yellow", "gray")
   
   wl_append_menu ()
   
   wl_rundown_task ()

   wl_select_menu_entity ("Music")
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
