function init ()
	wl_terminal_text_mode ()
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
	ta = {
		"ACI",  "ADC", "ADD", "ADI",  "ANA", "ANI", "CALL",   "CC",   "CM",  "CMA", 
		"CMC",  "CMP", "CNC", "CNZ",   "CP", "CPE",  "CPI",  "CPO",   "CZ",  "DAA"
	}
	tb = {	
		"DAD",  "DCR", "DCX",  "DI",   "EI", "HLT",   "IN",  "INR",  "INX",   "JC",
		"JM",   "JMP", "JNC", "JNZ",   "JP", "JPE",  "JPO",   "JZ",  "LDA", "LDAX"
	}
	tc = {
		"LHLD", "LXI", "MOV", "MVI",  "NOP", "ORA",  "ORI",  "OUT", "PCHL",  "POP",
		"PUSH", "RAL", "RAR",  "RC",  "RET", "RLC",   "RM",  "RNC",  "RNZ",   "RP"
	}
	td = {
		"RPE",  "RPO", "RRC", "RST",   "RZ", "SBB",  "SBI", "SHLD", "SPHL",  "STA",
		"STAX", "STC", "SUB", "SUI", "XCHG", "XRA",  "XRI", "XTHL", "GOTO", "back"
	}

	table = {}

	table [0] = ta
	table [1] = tb
	table [2] = tc
	table [3] = td

	u = 8	-- offset y
	v = 40	-- offset x
	m = 20	-- rows
	n = 3	-- columns
	s = 16	-- base ordinate for y
	x = 8   -- base ordinate for x
	y = s	-- initial height from where to beginn with a column

	wl_create_menu ("pause", x, y, "frame.png")

	for c = 0,n do
		y = s
		for r = 1,m do			
			wl_create_entity (
				2,
				x,
				y,
				table [c][r],
				"script/assembler/instruction/" .. table [c][r] .. ".lua",
				"white",
				"red",
				"yellow",
				"gray"
			)
			y = y + u
		end
		x = x + v
	end

	wl_append_menu ()

	wl_rundown_task ()

	wl_select_menu_entity (table [0][1])
end

function on_key_x ()
	
end

function on_key_y ()
	
end

function on_key_start ()
	
end

function on_key_back ()
	
end

init ()
