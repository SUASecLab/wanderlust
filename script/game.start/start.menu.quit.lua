function init ()

end

function on_key_up ()
   wl_select_menu_entity ("Option")
end

function on_key_down ()

end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_create_menu ("decision", 96, 120, "frame.png")
   
   wl_create_entity (2,96,120, "Yes", "script/game.start/start.menu.quit.yes.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2,96,136, "No",  "script/game.start/start.menu.quit.no.lua", "white", "red", "yellow", "gray")
   
   wl_append_menu ()
   
   wl_rundown_task ()

   wl_select_menu_entity ("Yes")

   -- wl_repeal_menu ()
   -- wl_leave_state_by_transition ("transition.fade.out.png", 16)
   -- wl_create_transition ("transition.fade.in.png", 16)

end

function on_key_b ()
   wl_repeal_menu ()
end

init ()
