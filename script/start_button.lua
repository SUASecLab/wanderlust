function init ()

end

function on_key_up ()

end

function on_key_down ()
   wl_select_state_entity ("Continue")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
    --wl_enter_state_by_transition (0, "game.state.lua", "transition.fade.out.png", 16)
    --wl_create_transition ("transition.fade.in.png", 16)
    wl_enter_state (1, "script/menu.state.lua")
end

init ()