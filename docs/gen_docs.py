import sys
sys.path.append("..")
from command_specification import * 

base_template = """
<html>
<head>
<title>rcp_docs</title>
</head>
<body>

{command_name_list}

{command_list}

</body>
</html>
"""

w = 6
ct = 0

segment = '<table>\n'
segment += '<tr>\n'
for cmd in commandList:
	segment += '<td>'+cmd['name']+'</td>\n'
	ct +=1
	if ct == w:
		ct = 0
		segment += '</tr>\n'
		segment += '<tr>\n'


segment += '</tr>\n'
segment += '</table>\n'

command_name_list = segment


segment_template = """
<div>
<b>{command_name}</b> [{command_args}] </br>
{command_short_description}
<div/>
"""

segment = ''
for cmd in commandList:
	if 'parameters' in cmd:
		sub_segment = ','.join(map(lambda elem:elem['name'],
					cmd['parameters']))
	segment += segment_template.format(
			command_name = cmd['name'],
			command_args = sub_segment,
			command_short_description = cmd['shortDescription'])

command_list = segment

with open('index.html','w') as f:
	f.write(base_template.format(
				command_name_list = command_name_list,
				command_list = command_list))

print (base_template.format(
			command_name_list = command_name_list,
			command_list = command_list))
