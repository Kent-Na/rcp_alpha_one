import re

typeTable = [
#	[id,	name,		c_type
#	 init, 	deinit,	copy, 	compare, write_json, send_as_command,
#	 set,	append,	unset, at]
#someting special
	[0,		'null',		None,				'0',
	 False,	False, False, False,  True, False,
	 False, False, False, False],
	[1,		'ref',		'rcp_record_ref',	'sizeof(rcp_record_ref*)',
	  True,  True,  True, False,  True, False,
	  True, False, False,  True],
#	[2,		used in internal],

#16-24 container
	[16,	'map',		'rcp_record_ref',	'sizeof(struct rcp_map_core)',
	  True,  True, False, False,  True, False,
	 False, False, False, False],
#	[17,	'set',		None],
#	 False,	False,	False, False, True],
	[18,	'old_array',
		'rcp_record_ref',	'sizeof(struct rcp_old_array_core)',
	  True,  True, False, False, True,  True,
	  True,  True, False,  True],
#	[19,	'struct',	None],
#	 False,	False,	False, False, True],

	[20,	'string',	'rcp_record_ref', 'sizeof(struct rcp_string_core)',
	 True,	True,	True, True, True, False,
	 False, False, False, False],
#	[21,	'binaly',	None],
#	 False,	False,	False, False, True],

#25-31 bool
	[25,	'bool8',	'uint8_t',	'1',
	 False,	False,	True, False, True, False,
	 False, False, False, False],
	[26,	'bool32',	'uint32_t',	'4',
	 False,	False,	True, False, True, False,
	 False, False, False, False],

#32-63 number
#32-39 uint
	[32,	'uint8',	'uint8_t',	'1',
	 False,	False,	True,  True, True, False,
	 False, False, False, False],
	[33,	'uint16',	'uint16_t',	'2',
	 False,	False,	True,  True, True, False,
	 False, False, False, False],
	[34,	'uint32',	'uint32_t',	'4',
	 False,	False,	True,  True, True, False,
	 False, False, False, False],
	[35,	'uint64',	'uint64_t',	'8',
	 False,	False,	True,  True, True, False,
	 False, False, False, False],

#40-47 int
	[40,	'int8',		'int8_t',	'1',
	 False,	False,	True,  True, True, False,
	 False, False, False, False],
	[41,	'int16',	'int16_t',	'2',
	 False,	False,	True,  True, True, False,
	 False, False, False, False],
	[42,	'int32',	'int32_t',	'4',
	 False,	False,	True,  True, True, False,
	 False, False, False, False],
	[43,	'int64',	'int64_t',	'8',
	 False,	False,	True,  True, True, False,
	 False, False, False, False],

#48-55 float
#	[48,	'half',		'half'],
	[49,	'float',	'float',	'4',
	 False,	False,	True,  True, True, False,
	 False, False, False, False],
	[50,	'double',	'double',	'8',
	 False,	False,	True,  True, True, False,
	 False, False, False, False],

#	[id,	name,		c_type
#	 init, 	deinit,		copy, 	compare, write_json]
]

#for internal use
exTypeTable = [
#	[id,	name,		c_type]
	[2,		'pointer',	'void*',	'sizeof(void*)',
	 False,	False,	True,  True, False, False,
	 False, False, False, False],
]


typeList = []
typeDict = {}

def tableToDict(info):
	typeInfoDict = {};
	typeInfoDict['typeID'] = typeInfo[0]
	typeInfoDict['typeName'] = typeInfo[1]
	typeInfoDict['cTypeName'] = typeInfo[2]
	typeInfoDict['dataSize'] = typeInfo[3]
	def f(f,tn,fn):
		if (not f):
			return 'NULL'
		else:
		 	return 'rcp_{tn}_{fn}'.format(tn = tn,fn = fn)

	funcList = [
		'init', 'deinit', 'copy', 'compare', 'writeJson', 
		'sendAsCommand', 'set', 'append', 'unset', 'at']

	idx = 4
	for func in funcList:
		name = '_'.join(re.findall(r'[A-Z]*[a-z]+',func)).lower()
		typeInfoDict[func+'Func'] = f(typeInfo[idx], typeInfo[1], name)
		idx = idx +1
	return typeInfoDict


for typeInfo in typeTable:
	typeInfoDict = tableToDict(typeInfo);
	typeList.append(typeInfoDict)
	typeDict[typeInfo[1]] = typeInfoDict

exTypeList = []
for typeInfo in exTypeTable:
	typeInfoDict = tableToDict(typeInfo);
	exTypeList.append(typeInfoDict);

numberTypeList = [] 
for typeInfo in typeList:
	if typeInfo['typeID'] & 0x20:
		numberTypeList.append(typeInfo)

for typeInfo in exTypeList:
	numberTypeList.append(typeInfo)

