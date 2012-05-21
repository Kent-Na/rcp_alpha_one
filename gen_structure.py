
#cStruct
cStructTemplate = """
struct {cStructName} {{
	{cStructFields}	
}}
"""

cStructFieldTemplate = """
	{cTypeName} {parameterName};
"""

#rcp_structure
structureTemplate = """
struct {{
	struct rcp_structure_core core;
	struct rcp_structure_parameter param[{parameterCount}];
}} {structureVariableName} = {{
	{{
		"{structureReverthDomainName}",
		{parameterCount}
	}},
	{{
{structureParameters}
	}}
}}
"""

structureParameterTemplate = """
		{{
			"{parameterName}",
			field_sizeof(struct {cStructName}, {parameterName}),
			offsetof(struct {cStructName}, {parameterName})
		}},
"""

cStructName = 'pm_task'
parameters = [
	{
		'parameterName':'name',
		'cTypeName':'uint32_t',
	}
]

parameterCount = len(parameters)
structureVariableName = cStructName + "_structure"
structureReverthDomainName = cStructName + ".tuna-cat.com"

cStructFields = "".join(map(lambda s:cStructFieldTemplate.format(**s),
			parameters))
structureParameters = "".join(map(lambda s:structureParameterTemplate.
			format(cStructName = cStructName, **s),parameters))


print (structureTemplate.format(
			structureParameters = structureParameters,
			parameterCount = parameterCount,
			structureVariableName = structureVariableName,
			cStructName = cStructName,
			structureReverthDomainName = structureReverthDomainName
			)
	  )

print (cStructTemplate.format(
			structureParameters = structureParameters,
			cStructFields = cStructFields,
			cStructName = cStructName,
			)
	  )
