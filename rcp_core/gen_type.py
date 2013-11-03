import re

typeTable = [
#old
#	[id,	name,		c_type
#	 init, 	deinit,	copy, 	compare, write_json, send_as_command,
#	 set,	append,	unset, at]

#new
#	[id,	name,    c_type, c_size,
#	 init, 	deinit,	 copied, compare, write_json, at, replace, merge, unset]

#0-15 someting special
	[0,	    'null', 	None,               '0',
	 False,	False, False, False,  True, False, False, False, False],
	[1,		'ref',		'rcp_record_ref',	'sizeof(rcp_record_ref*)',
	  True,	 True,  True, False,  True,  True, False, False, False],

#for internal use
	[2,		'pointer',	'void*',	        'sizeof(void*)',
	 False,	False, False,  True, False, False, False, False, False],

#16-24 container
	[20,	'string',	'rcp_record_ref', 'sizeof(struct rcp_string_core)',
	  True,	 True,  True,  True,  True, False, False, False, False],

#25-31 bool
	[25,	'bool8',	'uint8_t',	'1',
	 False,	False, False, False,  True, False, False, False, False],
	[26,	'bool32',	'uint32_t',	'4',
	 False,	False, False, False,  True, False, False, False, False],

#32-63 number
#32-39 uint
	[32,	'uint8',	'uint8_t',	'1',
	 False,	False, False,  True,  True, False, False, False, False],
	[33,	'uint16',	'uint16_t',	'2',
	 False,	False, False,  True,  True, False, False, False, False],
	[34,	'uint32',	'uint32_t',	'4',
	 False,	False, False,  True,  True, False, False, False, False],
	[35,	'uint64',	'uint64_t',	'8',
	 False,	False, False,  True,  True, False, False, False, False],

#40-47 int
	[40,	'int8',		'int8_t',	'1',
	 False,	False, False,  True,  True, False, False, False, False],
	[41,	'int16',	'int16_t',	'2',
	 False,	False, False,  True,  True, False, False, False, False],
	[42,	'int32',	'int32_t',	'4',
	 False,	False, False,  True,  True, False, False, False, False],
	[43,	'int64',	'int64_t',	'8',
	 False,	False, False,  True,  True, False, False, False, False],

#48-55 float
#	[48,	'half',		'half'],
	[49,	'float',	'float',	'4',
	 False,	False, False,  True,  True, False, False, False, False],
	[50,	'double',	'double',	'8',
	 False,	False, False,  True,  True, False, False, False, False],

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
		 	return 'rcp_default_{fn}'.format(fn = fn)
		else:
		 	return 'rcp_{tn}_{fn}'.format(tn = tn,fn = fn)

	funcList = [
        'init', 'deinit', 'copied', 'compare', 'writeJson', 
        'at', 'replace', 'merge', 'unset']

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

numberTypeList = [] 
for typeInfo in typeList:
	if typeInfo['typeID'] == 0x02: #pointer type
		numberTypeList.append(typeInfo)
	if typeInfo['typeID'] & 0x20:
		numberTypeList.append(typeInfo)

