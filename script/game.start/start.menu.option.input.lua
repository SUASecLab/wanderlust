function init ()

end

function on_key_up ()

end

function on_key_down ()
   wl_select_menu_entity ("Audio")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_create_menu ("input", 152, 104, "frame.png")
   
   wl_create_entity (2, 152,104, "Keyboard", "script/game.start/start.menu.option.input.keyboard.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 152,120, "Gamepad", "script/game.start/start.menu.option.input.gamepad.lua", "white", "red", "yellow", "gray")

   wl_append_menu ()
   
   wl_rundown_task ()

   wl_select_menu_entity ("Keyboard")
end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
