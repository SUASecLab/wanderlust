function init ()

end

function on_key_up ()
   wl_select_state_entity ("Resume")
end

function on_key_down ()

end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
   wl_leave_state_by_transition ("transition.fade.out.png", 16)
   wl_create_transition ("transition.fade.in.png", 16)
   wl_leave_state_by_transition ("transition.fade.out.png", 16)
   wl_create_transition ("transition.fade.in.png", 16)
end

init ()
