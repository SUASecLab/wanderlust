function init ()

end

function on_key_up ()
	wl_assembler_dcr_cursor ()
end

function on_key_down ()
	wl_assembler_inr_cursor ()
end

function on_key_left ()
	wl_select_menu_entity ("EXEC")
end

function on_key_right ()
	wl_select_menu_entity ("DATA")
	wl_terminal_data_mode ()
end

function on_key_a ()
	wl_create_menu ("pause", 8, 8, "frame.png")

	wl_create_entity (2, 8, 8, "ACI", "script/assembler/instruction/ACI.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,16, "ADC", "script/assembler/instruction/ADC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,24, "ADD", "script/assembler/instruction/ADD.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,32, "ADI", "script/assembler/instruction/ADI.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,40, "ANA", "script/assembler/instruction/ANA.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,48, "ANI", "script/assembler/instruction/ANI.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,56, "CALL", "script/assembler/instruction/CALL.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,64, "CC", "script/assembler/instruction/CC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,72, "CM", "script/assembler/instruction/CM.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,80, "CMA", "script/assembler/instruction/CMA.lua", "white", "red", "yellow", "gray")

	wl_create_entity (2, 8,88, "CMC", "script/assembler/instruction/CMC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,96, "CMP", "script/assembler/instruction/CMP.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,104, "CNC", "script/assembler/instruction/CNC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,112, "CNZ", "script/assembler/instruction/CNZ.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,120, "CP", "script/assembler/instruction/CP.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,128, "CPE", "script/assembler/instruction/CPE.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,136, "CPI", "script/assembler/instruction/CPI.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,144, "CPO", "script/assembler/instruction/CPO.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,152, "CZ", "script/assembler/instruction/CZ.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 8,160, "DAA", "script/assembler/instruction/DAA.lua", "white", "red", "yellow", "gray")

	wl_create_entity (2, 48,8, "DAD", "script/assembler/instruction/DAD.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 48,16, "DCR", "script/assembler/instruction/DCR.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 48,24, "DCX", "script/assembler/instruction/DCX.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 48,32, "DI", "script/assembler/instruction/DI.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 48,40, "EI", "script/assembler/instruction/EI.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 48,48, "HLT", "script/assembler/instruction/HLT.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 48,56, "IN", "script/assembler/instruction/IN.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 48,64, "INR", "script/assembler/instruction/INR.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 48,72, "INX", "script/assembler/instruction/INX.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 48,80, "JC", "script/assembler/instruction/JC.lua", "white", "red", "yellow", "gray")

	wl_create_entity (2,  48,88, "JM", "script/assembler/instruction/JM.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  48,96, "JMP", "script/assembler/instruction/JMP.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  48,104, "JNC", "script/assembler/instruction/JNC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  48,112, "JNZ", "script/assembler/instruction/JNZ.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  48,120, "JP", "script/assembler/instruction/JP.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  48,128, "JPE", "script/assembler/instruction/JPE.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  48,136, "JPO", "script/assembler/instruction/JPO.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  48,144, "JZ", "script/assembler/instruction/JZ.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  48,152, "LDA", "script/assembler/instruction/LDA.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  48,160, "LDAX", "script/assembler/instruction/LDAX.lua", "white", "red", "yellow", "gray")
	
	wl_create_entity (2,  88, 8, "LHLD", "script/assembler/instruction/LHLD.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,16, "LXI", "script/assembler/instruction/LXI.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,24, "MOV", "script/assembler/instruction/MOV.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,32, "MVI", "script/assembler/instruction/MVI.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,40, "NOP", "script/assembler/instruction/NOP.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,48, "ORA", "script/assembler/instruction/ORA.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,56, "ORI", "script/assembler/instruction/ORI.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,64, "OUT", "script/assembler/instruction/OUT.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,72, "PCHL", "script/assembler/instruction/PCHL.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,80, "POP", "script/assembler/instruction/POP.lua", "white", "red", "yellow", "gray")

	wl_create_entity (2,  88, 88, "PUSH", "script/assembler/instruction/PUSH.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88, 96, "RAL", "script/assembler/instruction/RAL.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,104, "RAR", "script/assembler/instruction/RAR.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,112, "RC", "script/assembler/instruction/RC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,120, "RET", "script/assembler/instruction/RET.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,128, "RLC", "script/assembler/instruction/RLC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,136, "RM", "script/assembler/instruction/RM.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,144, "RNC", "script/assembler/instruction/RNC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,152, "RNZ", "script/assembler/instruction/RNZ.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2,  88,160, "RP", "script/assembler/instruction/RP.lua", "white", "red", "yellow", "gray")
	
	wl_create_entity (2, 128,  8, "RPE", "script/assembler/instruction/RPE.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128, 16, "RPO", "script/assembler/instruction/RPO.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128, 24, "RRC", "script/assembler/instruction/RRC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128, 32, "RST", "script/assembler/instruction/RST.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128, 40, "RZ", "script/assembler/instruction/RZ.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128, 48, "SBB", "script/assembler/instruction/SBB.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128, 56, "SBI", "script/assembler/instruction/SBI.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128, 64, "SHLD", "script/assembler/instruction/SHLD.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128, 72, "SPHL", "script/assembler/instruction/SPHL.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128, 80, "STA", "script/assembler/instruction/STA.lua", "white", "red", "yellow", "gray")

	wl_create_entity (2, 128, 88, "STAX", "script/assembler/instruction/STAX.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128, 96, "STC", "script/assembler/instruction/STC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128,104, "SUB", "script/assembler/instruction/SUB.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128,112, "SUI", "script/assembler/instruction/SUI.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128,120, "XCHG", "script/assembler/instruction/XCHG.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128,128, "XRA", "script/assembler/instruction/XRA.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128,136, "XRI", "script/assembler/instruction/XRI.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128,144, "XTHL", "script/assembler/instruction/XTHL.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128,152, "GOTO", "script/assembler/instruction/GOTO.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 128,160, "back", "script/assembler/back.lua", "white", "red", "yellow", "gray")

	wl_append_menu ()

	wl_rundown_task ()

	wl_select_menu_entity ("ACI")
end

function on_key_x ()
	wl_increment_cursor_terminal ()
end

function on_key_y ()
	wl_decrement_cursor_terminal ()
end

function on_key_start ()
	wl_run_terminal ()
end

function on_key_back ()
	wl_clear_terminal ()
end

init ()
