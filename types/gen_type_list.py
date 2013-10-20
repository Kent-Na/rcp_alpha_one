from gen_type import *

#
# generate program(number)
#

numberCBaseFileName = 'rcp_number'
outNumberCFile = open(numberCBaseFileName+'.c','w')
outNumberCHeader = open(numberCBaseFileName+'.h','w')

listCBaseFileName = 'rcp_type_list'
outListCFile = open(listCBaseFileName+'.c','w')
outListCHeader = open(listCBaseFileName+'.h','w')

numberTypeTemplate="""
int rcp_{typeName}_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){{
	if (*({cTypeName}*)l<*({cTypeName}*)r)
		return -1;
	if (*({cTypeName}*)l>*({cTypeName}*)r)
		return +1;
	return 0;
}}
"""

numberTypeHeaderTemplate="""
rcp_extern void rcp_{typeName}_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst);
rcp_extern int rcp_{typeName}_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r);
"""

outNumberCFile.write('//This file was generated by "gen_type_list.py".\n')
outNumberCFile.write('#include "../rcp_pch.h"\n')
outNumberCFile.write('#include "../rcp_utility.h"\n')
outNumberCFile.write('#define RCP_INTERNAL_STRUCTURE\n')
outNumberCFile.write('#include "rcp_type.h"\n')
outNumberCFile.write('#include "rcp_type_list.h"\n')

for info in numberTypeList:
	outNumberCFile.write(numberTypeTemplate.format(
				capName = info['typeName'].upper(),
				**info))

outNumberCHeader.write('//This file was generated by "gen_type_list.py".\n')
outNumberCHeader.write('#include "../def/rcp_type.h"\n')
outNumberCHeader.write('#include "../def/rcp_data.h"\n')
for info in numberTypeList:
	outNumberCHeader.write(numberTypeHeaderTemplate.format(**info))

#
# generate program(genelic)
#

typeDefTemplate="""
struct rcp_type_core rcp_{typeName}_type_def = {{
	{dataSize},
	{typeID},
	NULL,
	{initFunc},
	{deinitFunc},
	{copiedFunc},
	{compareFunc},
	{writeJsonFunc},
	{atFunc},
	{replaceFunc},
	{mergeFunc},
}};
"""

typeTemplate="""
const rcp_type_ref rcp_{typeName}_type = &rcp_{typeName}_type_def;
"""

typeHeaderTemplate="""
#define RCP_TYPE_{capName} ((rcp_type_id_t){typeID})
extern const rcp_type_ref rcp_{typeName}_type;
"""

typeHeaderTemplateInternal="""
extern struct rcp_type_core rcp_{typeName}_type_def;
"""

outListCFile.write('//This file was generated by "gen_type_list.py".\n')
outListCFile.write('#include "../rcp_pch.h"\n');
outListCFile.write('#include "../rcp_utility.h"\n');
outListCFile.write('#define RCP_INTERNAL_STRUCTURE\n');
outListCFile.write('#include "rcp_tree.h"\n');
outListCFile.write('#include "rcp_type.h"\n');
outListCFile.write('#include "rcp_json_write.h"\n');
outListCFile.write('#include "../rcp_send_as_command.h"\n');

outListCFile.write('#include "rcp_number.h"\n');
outListCFile.write('#include "rcp_string.h"\n');
outListCFile.write('#include "rcp_type_etc.h"\n');

for info in typeList:
	outListCFile.write(typeDefTemplate.format(**info))
for info in typeList:
	outListCFile.write(typeTemplate.format(**info))


#########################
#header
#

outListCHeader.write('//This file was generated by "gen_type_list.py".\n')
outListCHeader.write('#include "../def/rcp_type.h"\n');
for info in typeList:
	outListCHeader.write(typeHeaderTemplate.format(
				capName = info['typeName'].upper(),
				**info))

outListCHeader.write('#ifdef RCP_INTERNAL_STRUCTURE\n');
for info in typeList:
	outListCHeader.write(typeHeaderTemplateInternal.format(
				capName = info['typeName'].upper(),
				**info))

outListCHeader.write('#endif\n');
