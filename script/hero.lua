function on_key_x ()
   wl_create_textbox ("white", "\nThere is no Item assigned to\nthis button...", "frame.png")
end

function on_key_y ()
   wl_create_terminal ("frame.png", "script/assembler/assembler.lua", "red", "white", 24, 16)
   wl_rundown_task ()
   wl_show_terminal ()
   --wl_create_textbox ("white", "You here a voice from beyond!\n\nIt is not the right time for\nthis item to be used.", "frame.png")
   --wl_create_terminal ("frame.png", "script/brainfuck/terminal.lua", "red", "white", 32, 16)
   --wl_rundown_task ()
   --wl_show_terminal ()
end

function on_key_start ()
   wl_create_menu ("pause", 8, 8, "frame.png")
   
   wl_create_entity (2, 8,8, "Pokedex", "script/game.start/start.menu.pokedex.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 8,24, "Pokemon", "script/game.start/start.menu.pokemon.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 8,40, "Inventory", "script/game.start/start.menu.inventory.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 8,56, "PokeCom", "script/game.start/start.menu.pokecom.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 8,72, "Trainer", "script/game.start/start.menu.trainer.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 8,88, "Save", "script/game.start/start.menu.save.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 8,104, "Option", "script/game.start/start.menu.option.lua", "white", "red", "yellow", "gray")
   wl_create_entity (2, 8,120, "Quit", "script/game.start/start.menu.quit.lua", "white", "red", "yellow", "gray")
   
   wl_append_menu ()
   
   wl_rundown_task ()

   wl_select_menu_entity ("Pokedex")
end

function on_key_back ()
   wl_enter_state_by_transition (1, "script/pause.state.lua", "transition.fade.out.png", 16)
   wl_create_transition ("transition.fade.in.png", 16)
end

function init ()

end

init ()