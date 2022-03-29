function init (x, y)
	wl_create_menu ("assembler", x+8, y+8, "frame.png")
   
   	wl_create_entity (2, x +   8, y + 8, "EXEC", "script/assembler/RUN.lua", "white", "red", "yellow", "gray")
   	wl_create_entity (2, x +  48, y + 8, "TEXT", "script/assembler/IN.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 88, y + 8, "DATA", "script/assembler/BYTE.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 128, y + 8, "GOTO", "script/assembler/JUMP.lua", "gray", "red", "yellow", "gray")
	wl_create_entity (2, x + 168, y + 8, "BACK", "script/assembler/END.lua", "white", "red", "yellow", "gray")

	wl_append_menu ()

	wl_rundown_task ()

	wl_select_menu_entity ("EXEC")
end