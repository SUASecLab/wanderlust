function init ()

end

function on_key_up ()
	wl_select_menu_entity ("JMP")
end

function on_key_down ()
   wl_select_menu_entity ("JNZ")
end

function on_key_left ()
	wl_select_menu_entity ("CNC")
end

function on_key_right ()
	wl_select_menu_entity ("RAR")
end

function on_key_a ()
	wl_mnemonic_set_base (0xD2)
	wl_create_menu ("byte_a", 176, 24, "frame.png")
	wl_create_entity (2, 176, 24, "0", "script/assembler/word/a/NA0.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 32, "1", "script/assembler/word/a/NA1.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 40, "2", "script/assembler/word/a/NA2.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 48, "3", "script/assembler/word/a/NA3.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 56, "4", "script/assembler/word/a/NA4.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 64, "5", "script/assembler/word/a/NA5.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 72, "6", "script/assembler/word/a/NA6.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 80, "7", "script/assembler/word/a/NA7.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 88, "8", "script/assembler/word/a/NA8.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 96, "9", "script/assembler/word/a/NA9.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 104, "A", "script/assembler/word/a/NAA.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 112, "B", "script/assembler/word/a/NAB.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 120, "C", "script/assembler/word/a/NAC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 128, "D", "script/assembler/word/a/NAD.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 136, "E", "script/assembler/word/a/NAE.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 144, "F", "script/assembler/word/a/NAF.lua", "white", "red", "yellow", "gray")
	wl_append_menu ()
	wl_rundown_task ()
	wl_select_menu_entity ("0")
end

function on_key_b ()
	wl_repeal_menu ()
end

function on_key_x ()

end

function on_key_y ()
	wl_create_textbox (
      "white", 
      "JUMP IF NO CARRY              " ..
      "                              " ..
      "The INC instruction tests the " ..
      "setting of the carry flag. If " ..
      "there is no carry:            " ..
      "               carry flag = 0," ..
      "program execution resumes at  " ..
      "the address specified in the  " ..
      "INC instruction. If there is a" ..
      "carry (carry flag = 1), exe-  " ..
      "cution continues with the next" ..
      "sequential Instruction.       " ..
      "The address may be specified  " ..
      "as a number, a label, or an   " ..
      "expression. The assembler in- " ..
      "verts the high and low address" ..
      "byte; when it assembles the   " ..
      "instruction.                  ",
      "frame.png"
   )
end

function on_key_start ()
	
end

function on_key_back ()
	
end

init ()
