function init ()

end

function on_key_up ()
   wl_select_state_entity ("Terminal")
end

function on_key_down ()
   wl_select_state_entity ("Quit")
end

function on_key_left ()

end

function on_key_right ()

end

function on_key_a ()
	wl_create_terminal ("frame.png", "script/assembler/assembler.lua", "red", "white", 24, 16)
	wl_rundown_task ()
	wl_show_terminal ()
end

init ()
