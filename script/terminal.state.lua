function on_key_back ()
   wl_enter_state_by_transition (1, "script/pause.state.lua", "transition.fade.out.png", 16)
   wl_create_transition ("transition.fade.in.png", 16)
end

function init ()   
   
   wl_rundown_task ()

   wl_fetch_state ()

   wl_rundown_task ()

--   set_background ("title.png")
end

init ()
