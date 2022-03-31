function init ()

end

function on_key_up ()
   --wl_select_menu_entity ("Save")
end

function on_key_down ()
   wl_select_menu_entity ("Quit")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_create_menu ("option", 96, 104, "frame.png")
   
   wl_create_entity (2, 96,104, "Input", "script/game.start/start.menu.option.input.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 96,120, "Audio", "script/game.start/start.menu.option.audio.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 96,136, "Video", "script/game.start/start.menu.option.video.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 96,152, "Story", "script/game.start/start.menu.option.story.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 96,168, "Reset", "script/game.start/start.menu.option.reset.lua", "white", "red", "yellow", "gray")

   wl_append_menu ()
   
   wl_rundown_task ()

   wl_select_menu_entity ("Input")
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
