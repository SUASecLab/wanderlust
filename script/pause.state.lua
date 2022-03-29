function init ()   
   wl_create_entity (2, 8,8, "Resume", "script/menu.pause/pause.button.resume.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 8,16, "Quit", "script/menu.pause/pause.button.quit.lua", "white", "red", "yellow", "gray")

   wl_fetch_state ()

   wl_rundown_task ()

   wl_select_state_entity ("Resume")
end

init ()
