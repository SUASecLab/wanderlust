function init ()

end

function on_key_up ()
	wl_select_menu_entity ("JPE")
end

function on_key_down ()
   wl_select_menu_entity ("JZ")
end

function on_key_left ()
	wl_select_menu_entity ("CPI")
end

function on_key_right ()
	wl_select_menu_entity ("RM")
end

function on_key_a ()
	wl_mnemonic_set_base (0xE2)
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
      "JUMP IF PARITY ODD            " ..
      "                              " ..
      "Parity is odd whne the byte in" ..
      "the accumulator has a number  " ..
      "of one bits that is not even. " ..
      "The parity flag is set to zero" ..
      "to indicate this condition.   " ..
      "The JPO Instruction tests the " ..
      "setting of the parity flag. If" ..
      "the parity flag is reset to   " ..
      "zero, program execution re-   " ..
      "sumes at the address specified" ..
      "in the JPO instruction. If the" ..
      "flag is set to one. execution " ..
      "continues with the next seq-  " ..
      "quential instruction.         " ..
      "The address may be specified  " ..
      "as a number. a label. or an   " ..
      "expression. The assembler in- " ..
      "verts the high and low address" ..
      "bytes when it assembles the   " ..
      "instruction.                  " ..
      "The JPO and JPE (JMP if parity" ..
      "even) instructions are es-    " ..
      "pecially useful for testing   " ..
      "the parity of Input data. How-" ..
      "ever. the IN instruction does " ..
      "not alter any of the condition" ..
      "flags. The flags can be set by" ..
      "adding OOH to the contents of " ..
      "the accumulator.",
      "frame.png"
   )
end

function on_key_start ()

end

function on_key_back ()

end

init ()
