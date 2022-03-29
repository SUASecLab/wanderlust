function init ()

end

function on_key_up ()
  wl_select_state_entity ("New Game")
end

function on_key_down ()
   wl_select_state_entity ("Options")
end

function on_key_a ()
   wl_create_textbox ("white", "Before revisiting a game that\nis non existent. How about you\nstart by creating a new one.\nSee you in a few minutes...", "frame.png")
end

init ()
