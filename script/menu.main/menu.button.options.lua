function init ()

end

function on_key_up ()
   wl_select_state_entity ("Continue")
end

function on_key_down ()
   wl_select_state_entity ("Terminal")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_create_menu ("pause", 8, 88, "frame.png")
   
   wl_create_entity (2, 8, 88, "Handbooks", "script/game.start/start.menu.save.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 8,104, "Option", "script/game.start/start.menu.option.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 8,120, "Quit", "script/game.start/start.menu.quit.lua", "white", "red", "yellow", "gray")
   
   wl_append_menu ()
   
   wl_rundown_task ()

   wl_select_menu_entity ("Quit")
end

init ()
