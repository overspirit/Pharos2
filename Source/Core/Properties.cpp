#include "PreCompile.h"
#include "CoreGlobal.h"

Properties::Properties()
{
	_visited = false;
}

Properties::~Properties()
{
}

bool Properties::Open(const char8* path)
{
	if (path == nullptr) return false;

	File* file = sKernel->CreateFileStream(path);
	if (file == NULL) return false;

	readProperties(file);
	rewind();
	resolveInheritance();

	return true;
}

bool Properties::Init(File* file, const char8* name, const char8* id, const char8* parentID, Properties* parent)
{
	_namespace = name;
	_visited = false;
	_parent = parent;

	if (id)
	{
		_id = id;
	}

	if (parentID)
	{
		_parentID = parentID;
	}

	readProperties(file);
	rewind();

	return true;
}

void Properties::readProperties(File* file)
{
	char8 line[2048];
	char8 variable[256];
	int32 c;
	char8* name;
	char8* value;
	char8* parentID;
	char8* rc;
	char8* rcc;
	char8* rccc;
	bool comment = false;

	while (true)
	{
		// Skip whitespace at the start of lines
		skipWhiteSpace(file);

		// Stop when we have reached the end of the file.
		if (file->IsEndOfFile())
			break;

		// Read the next line.
		rc = readLine(file, line, 2048);

		if (rc == NULL) return;

		// Ignore comments
		if (comment)
		{
			// Check for end of multi-line comment at either start or end of line
			if (strncmp(line, "*/", 2) == 0)
				comment = false;
			else
			{
				trimWhiteSpace(line);
				const int32 len = (int32)strlen(line);
				if (len >= 2 && strncmp(line + (len - 2), "*/", 2) == 0)
					comment = false;
			}
		}
		else if (strncmp(line, "/*", 2) == 0)
		{
			// Start of multi-line comment (must be at start of line)
			comment = true;
		}
		else if (strncmp(line, "//", 2) != 0)
		{
			// If an '=' appears on this line, parse it as a name/value pair.
			// Note: strchr() has to be called before strtok(), or a backup of line has to be kept.
			rc = strchr(line, '=');
			if (rc != NULL)
			{
				// First token should be the property name.
				name = strtok(line, "=");
				if (name == NULL) return;

				// Remove white-space from name.
				name = trimWhiteSpace(name);

				// Scan for next token, the property's value.
				value = strtok(NULL, "");
				if (value == NULL) return;

				// Remove white-space from value.
				value = trimWhiteSpace(value);

				// Is this a variable assignment?
				if (isVariable(name, variable, 256))
				{
					SetVariable(variable, value);
				}
				else
				{
					// Normal name/value pair
					_properties.push_back(Property(name, value));
				}
			}
			else
			{
				parentID = NULL;

				// Get the last character on the line (ignoring whitespace).
				const char8* lineEnd = trimWhiteSpace(line) + (strlen(trimWhiteSpace(line)) - 1);

				// This line might begin or end a namespace,
				// or it might be a key/value pair without '='.

				// Check for '{' on same line.
				rc = strchr(line, '{');

				// Check for inheritance: ':'
				rcc = strchr(line, ':');

				// Check for '}' on same line.
				rccc = strchr(line, '}');

				// Get the name of the namespace.
				name = strtok(line, " \t\n{");
				name = trimWhiteSpace(name);
				if (name == NULL)
				{
					return;
				}
				else if (name[0] == '}')
				{
					// End of namespace.
					return;
				}

				// Get its ID if it has one.
				value = strtok(NULL, ":{");
				value = trimWhiteSpace(value);

				// Get its parent ID if it has one.
				if (rcc != NULL)
				{
					parentID = strtok(NULL, "{");
					parentID = trimWhiteSpace(parentID);
				}

				if (value != NULL && value[0] == '{')
				{
					// If the namespace ends on this line, seek back to right before the '}' character.
					if (rccc && rccc == lineEnd)
					{
						if (file->Seek(-1, EFST_CURR) == false) return;

						while (readChar(file) != '}')
						{
							if (file->Seek(-2, EFST_CURR) == false) return;
						}

						if (file->Seek(-1, EFST_CURR) == false) return;
					}

					// New namespace without an ID.
					Properties* space = new Properties();
					space->Init(file, name, (const char8*)NULL, parentID, this);
					_namespaces.push_back(space);

					// If the namespace ends on this line, seek to right after the '}' character.
					if (rccc && rccc == lineEnd)
					{
						if (file->Seek(1, EFST_CURR) == false) return;
					}
				}
				else
				{
					// If '{' appears on the same line.
					if (rc != NULL)
					{
						// If the namespace ends on this line, seek back to right before the '}' character.
						if (rccc && rccc == lineEnd)
						{
							if (file->Seek(-1, EFST_CURR) == false) return;

							while (readChar(file) != '}')
							{
								if (file->Seek(-2, EFST_CURR) == false) return;
							}

							if (file->Seek(-1, EFST_CURR) == false) return;
						}

						// Create new namespace.
						Properties* space = new Properties();
						space->Init(file, name, value, parentID, this);
						_namespaces.push_back(space);

						// If the namespace ends on this line, seek to right after the '}' character.
						if (rccc && rccc == lineEnd)
						{
							if (file->Seek(1, EFST_CURR) == false) return;
						}
					}
					else
					{
						// Find out if the next line starts with "{"
						skipWhiteSpace(file);
						c = readChar(file);
						if (c == '{')
						{
							// Create new namespace.
							Properties* space = new Properties();
							space->Init(file, name, value, parentID, this);
							_namespaces.push_back(space);
						}
						else
						{
							// Back up from fgetc()
							file->Seek(-1, EFST_CURR);

							// Store "name value" as a name/value pair, or even just "name".
							if (value != NULL)
							{
								_properties.push_back(Property(name, value));
							}
							else
							{
								_properties.push_back(Property(name, ""));
							}
						}
					}
				}
			}
		}
	}
}

void Properties::skipWhiteSpace(File* file)
{
	char8 c;
	do
	{
		c = readChar(file);
	} while (isspace(c) && c != EOF);

	// If we are not at the end of the file, then since we found a
	// non-whitespace character, we put the cursor back in front of it.
	if (c != EOF)
	{
		file->Seek(-1, EFST_CURR);
	}
}

char8* Properties::trimWhiteSpace(char8 *str)
{
	if (str == NULL) return str;

	char8* end = NULL;

	// Trim leading space.
	while (isspace(*str))
		str++;

	// All spaces?
	if (*str == 0)
	{
		return str;
	}

	// Trim trailing space.
	end = str + strlen(str) - 1;
	while (end > str && isspace(*end))
		end--;

	// Write new null terminator.
	*(end + 1) = 0;

	return str;
}

void Properties::resolveInheritance(const char8* id)
{
	// Namespaces can be defined like so:
	// "name id : parentID { }"
	// This method merges data from the parent namespace into the child.

	// Get a top-level namespace.
	Properties* derived;
	if (id)
	{
		derived = GetNamespace(id);
	}
	else
	{
		derived = GetNextNamespace();
	}

	while (derived)
	{
		// If the namespace has a parent ID, find the parent.
		if (!derived->_parentID.empty())
		{
			derived->_visited = true;
			Properties* parent = GetNamespace(derived->_parentID.c_str());
			if (parent)
			{
				resolveInheritance(parent->GetId());

				// Copy the child.
				Properties* overrides = derived->clone();

				// Copy data from the parent into the child.
				derived->_properties = parent->_properties;
				derived->_namespaces = std::vector< Properties* >();
				std::vector< Properties* >::const_iterator itt;
				for (itt = parent->_namespaces.begin(); itt < parent->_namespaces.end(); ++itt)
				{
					Properties* copyIter = (*itt);
					Properties* prop = copyIter->clone();
					derived->_namespaces.push_back(prop);
				}
				derived->rewind();

				// Take the original copy of the child and override the data copied from the parent.
				derived->mergeWith(overrides);
			}
			derived->_visited = false;
		}

		// Resolve inheritance within this namespace.
		derived->resolveInheritance();

		// Get the next top-level namespace and check again.
		if (!id)
		{
			derived = GetNextNamespace();
		}
		else
		{
			derived = NULL;
		}
	}
}

void Properties::mergeWith(Properties* overrides)
{
	// Overwrite or add each property found in child.
	overrides->rewind();
// 	const char8* name = overrides->GetNextPropertyName();
// 	while (name)
// 	{
// 		this->SetString(name, overrides->GetString());
// 		name = overrides->GetNextPropertyName();
// 	}
// 	this->_propertiesItr = this->_properties.end();
	for (uint32 i = 0; i < overrides->GetPropertyCount(); i++)
	{
		const char8* name = overrides->GetPropertyName(i);
		const char8* value = overrides->GetPropertyValue(i);
		this->_properties.push_back(Property(name, value));
	}

	// Merge all common nested namespaces, add new ones.
	Properties* overridesNamespace = overrides->GetNextNamespace();
	while (overridesNamespace)
	{
		bool merged = false;

		rewind();
		Properties* derivedNamespace = GetNextNamespace();
		while (derivedNamespace)
		{
			if (strcmp(derivedNamespace->GetName(), overridesNamespace->GetName()) == 0 &&
				strcmp(derivedNamespace->GetId(), overridesNamespace->GetId()) == 0)
			{
				derivedNamespace->mergeWith(overridesNamespace);
				merged = true;
			}

			derivedNamespace = GetNextNamespace();
		}

		if (!merged)
		{
			// Add this new namespace.
			Properties* newNamespace = overridesNamespace->clone();

			this->_namespaces.push_back(newNamespace);
			this->_namespacesItr = this->_namespaces.end();
		}

		overridesNamespace = overrides->GetNextNamespace();
	}
}

// Properties* Properties::GetNextProperty()
// {
// 	if (_propertiesItr == _properties.end())
// 	{
// 		// Restart from the beginning
// 		_propertiesItr = _properties.begin();
// 	}
// 	else
// 	{
// 		// Move to the next property
// 		++_propertiesItr;
// 	}
// 
// 	Properties* prop = *_propertiesItr->;
// }

uint32 Properties::GetPropertyCount() const
{
	return (uint32)_properties.size();
}

const char8* Properties::GetPropertyName(uint32 index) const
{
	return index < _properties.size() ? _properties[index].name.c_str() : "";
}

const char8* Properties::GetPropertyValue(uint32 index) const
{
	return index < _properties.size() ? _properties[index].value.c_str() : "";
}

const char8* Properties::GetPropertyValue(const char8* name) const
{
	for (std::vector<Property>::const_iterator itr = _properties.begin(); itr != _properties.end(); ++itr)
	{
		if (itr->name == name)
		{
			return itr->value.c_str();
		}
	}

	return "";
}

Properties* Properties::GetNextNamespace()
{
	if (_namespacesItr == _namespaces.end())
	{
		// Restart from the beginning
		_namespacesItr = _namespaces.begin();
	}
	else
	{
		++_namespacesItr;
	}

	if (_namespacesItr != _namespaces.end())
	{
		Properties* ns = *_namespacesItr;
		return ns;
	}

	return NULL;
}

void Properties::rewind()
{
	//_propertiesItr = _properties.end();
	_namespacesItr = _namespaces.end();
}

Properties* Properties::GetNamespace(const char8* id, bool searchNames) const
{
	for (std::vector< Properties* >::const_iterator it = _namespaces.begin(); it < _namespaces.end(); ++it)
	{
		Properties* p = *it;
		if (strcmp(searchNames ? p->_namespace.c_str() : p->_id.c_str(), id) == 0)
			return p;
	}

	return NULL;
}

const char8* Properties::GetName() const
{
	return _namespace.c_str();
}

const char8* Properties::GetId() const
{
	return _id.c_str();
}

bool Properties::Exists(const char8* name) const
{
	if (name == NULL)
		return false;

	for (std::vector<Property>::const_iterator itr = _properties.begin(); itr != _properties.end(); ++itr)
	{
		if (itr->name == name)
			return true;
	}

	return false;
}

PropType Properties::GetType(const char8* name) const
{
	const char8* value = GetString(name);
	if (!value)
	{
		return EPT_NONE;
	}

	// Parse the value to determine the format
	uint32 commaCount = 0;
	char8* valuePtr = const_cast<char8*>(value);
	while ((valuePtr = strchr(valuePtr, ',')))
	{
		valuePtr++;
		commaCount++;
	}

	switch (commaCount)
	{
	case 0:
		return isStringNumeric(value) ? EPT_NUMBER : EPT_STRING;
	case 1:
		return EPT_VECTOR2;
	case 2:
		return EPT_VECTOR3;
	case 3:
		return EPT_VECTOR4;
	case 15:
		return EPT_MATRIX;
	default:
		return EPT_STRING;
	}
}

const char8* Properties::GetString(const char8* name, const char8* defaultValue) const
{
	char8 variable[256];
	const char8* value = NULL;

	if (name)
	{
		// If 'name' is a variable, return the variable value
		if (isVariable(name, variable, 256))
		{
			return GetVariable(variable, defaultValue);
		}

		for (std::vector<Property>::const_iterator itr = _properties.begin(); itr != _properties.end(); ++itr)
		{
			if (itr->name == name)
			{
				value = itr->value.c_str();
				break;
			}
		}
	}
	else
	{
		// No name provided - get the value at the current iterator position
		//if (_propertiesItr != _properties.end())
		//{
		//	value = _propertiesItr->value.c_str();
		//}
	}

	if (value)
	{
		// If the value references a variable, return the variable value
		if (isVariable(value, variable, 256))
			return GetVariable(variable, defaultValue);

		return value;
	}

	return defaultValue;
}

bool Properties::SetString(const char8* name, const char8* value)
{
	if (name)
	{
		for (std::vector<Property>::iterator itr = _properties.begin(); itr != _properties.end(); ++itr)
		{
			if (itr->name == name)
			{
				// Update the first property that matches this name
				itr->value = value ? value : "";
				return true;
			}
		}

		// There is no property with this name, so add one
		_properties.push_back(Property(name, value ? value : ""));
	}
	else
	{
		// If there's a current property, set its value
		//if (_propertiesItr == _properties.end())
		//	return false;

		//_propertiesItr->value = value ? value : "";
	}

	return true;
}

bool Properties::GetBool(const char8* name, bool defaultValue) const
{
	const char8* valueString = GetString(name);
	if (valueString)
	{
		return (strcmp(valueString, "true") == 0);
	}

	return defaultValue;
}

int32 Properties::GetInt(const char8* name) const
{
	const char8* valueString = GetString(name);
	if (valueString)
	{
		int32 value;
		int32 scanned;
		scanned = sscanf(valueString, "%d", &value);
		if (scanned != 1)
		{
			return 0;
		}
		return value;
	}

	return 0;
}

float32 Properties::GetFloat(const char8* name) const
{
	const char8* valueString = GetString(name);
	if (valueString)
	{
		float32 value;
		int32 scanned;
		scanned = sscanf(valueString, "%f", &value);
		if (scanned != 1)
		{
			return 0.0f;
		}
		return value;
	}

	return 0.0f;
}

long Properties::GetLong(const char8* name) const
{
	const char8* valueString = GetString(name);
	if (valueString)
	{
		long value;
		int32 scanned;
		scanned = sscanf(valueString, "%ld", &value);
		if (scanned != 1)
		{
			return 0L;
		}
		return value;
	}

	return 0L;
}

bool Properties::GetMatrix(const char8* name, Matrix4* out) const
{
	const char8* valueString = GetString(name);
	if (valueString)
	{
		float32 m[16];
		int32 scanned;
		scanned = sscanf(valueString, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
			&m[0], &m[1], &m[2], &m[3], &m[4], &m[5], &m[6], &m[7],
			&m[8], &m[9], &m[10], &m[11], &m[12], &m[13], &m[14], &m[15]);

		if (scanned != 16)
		{
			out->MakeIdentity();
			return false;
		}

		out->SetData(m);

		return true;
	}

	out->MakeIdentity();
	return false;
}

bool Properties::GetVector2(const char8* name, Vector2Df* out) const
{
	return parseVector2(GetString(name), out);
}

bool Properties::GetVector3(const char8* name, Vector3Df* out) const
{
	return parseVector3(GetString(name), out);
}

bool Properties::GetVector4(const char8* name, Vector4Df* out) const
{
	return parseVector4(GetString(name), out);
}

bool Properties::GetQuaternionFromAxisAngle(const char8* name, Quaternion* out) const
{
	return parseAxisAngle(GetString(name), out);
}

bool Properties::GetColor(const char8* name, Color4* out) const
{
	return parseColor(GetString(name), out);
}

const char8* Properties::GetVariable(const char8* name, const char8* defaultValue) const
{
	if (name == NULL)
		return defaultValue;

	// Search for variable in this Properties object	
	for (size_t i = 0; i < _variables.size(); ++i)
	{
		const Property& prop = _variables[i];
		if (prop.name == name)
			return prop.value.c_str();
	}

	// Search for variable in parent Properties
	return (_parent != nullptr) ? _parent->GetVariable(name, defaultValue) : defaultValue;
}

void Properties::SetVariable(const char8* name, const char8* value)
{
	Property* prop = NULL;

	// Search for variable in this Properties object and parents
	Properties* current = const_cast<Properties*>(this);
	while (current)
	{
		for (size_t i = 0; i < current->_variables.size(); ++i)
		{
			Property& p = current->_variables[i];
			if (p.name == name)
			{
				prop = &p;
				break;
			}
		}

		current = current->_parent;
	}

	if (prop)
	{
		// Found an existing property, set it
		prop->value = value ? value : "";
	}
	else
	{
		// Add a new variable with this name
		_variables.push_back(Property(name, value ? value : ""));
	}
}

Properties* Properties::clone()
{
	Properties* p = new Properties();

	p->_namespace = _namespace;
	p->_id = _id;
	p->_parentID = _parentID;
	p->_properties = _properties;
	//p->_propertiesItr = p->_properties.end();

	for (size_t i = 0, count = _namespaces.size(); i < count; i++)
	{
		Properties* child = _namespaces[i]->clone();
		p->_namespaces.push_back(child);
		child->_parent = p;
	}

	p->_namespacesItr = p->_namespaces.end();

	return p;
}

bool Properties::parseVector2(const char8* str, Vector2Df* out)
{
	if (str)
	{
		float32 x, y;
		if (sscanf(str, "%f,%f", &x, &y) == 2)
		{
			if (out)
				out->Set(x, y);
			return true;
		}
	}

	if (out)
		out->Set(0.0f, 0.0f);

	return false;
}

bool Properties::parseVector3(const char8* str, Vector3Df* out)
{
	if (str)
	{
		float32 x, y, z;
		if (sscanf(str, "%f,%f,%f", &x, &y, &z) == 3)
		{
			if (out)
				out->Set(x, y, z);
			return true;
		}
	}

	if (out)
		out->Set(0.0f, 0.0f, 0.0f);
	return false;
}

bool Properties::parseVector4(const char8* str, Vector4Df* out)
{
	if (str)
	{
		float32 x, y, z, w;
		if (sscanf(str, "%f,%f,%f,%f", &x, &y, &z, &w) == 4)
		{
			if (out)
				out->Set(x, y, z, w);
			return true;
		}
	}

	if (out)
		out->Set(0.0f, 0.0f, 0.0f, 0.0f);

	return false;
}

bool Properties::parseAxisAngle(const char8* str, Quaternion* out)
{
	if (str)
	{
		float32 x, y, z, theta;
		if (sscanf(str, "%f,%f,%f,%f", &x, &y, &z, &theta) == 4)
		{
			if (out)
				out->FromAngleAxis((float32)(theta * DEGTORAD), Vector3Df(x, y, z));
			return true;
		}
	}

	if (out)
		out->Set(0.0f, 0.0f, 0.0f, 1.0f);

	return false;
}

bool Properties::parseColor(const char8* str, Color4* out)
{
	if (str)
	{
		if (strlen(str) == 7 && str[0] == '#')
		{
			// Read the string into an int32 as hex.
			uint32 color;
			if (sscanf(str + 1, "%x", &color) == 1)
			{
				if (out) *out = color;

				return true;
			}
		}
	}

	if (out) *out = 0x00000000;

	return false;
}

bool Properties::isStringNumeric(const char8* str)
{
	// The first character may be '-'
	if (*str == '-')
		str++;

	// The first character after the sign must be a digit
	if (!isdigit(*str))
		return false;

	str++;

	// All remaining characters must be digits, with a single decimal (.) permitted
	uint32 decimalCount = 0;
	while (*str)
	{
		if (!isdigit(*str))
		{
			if (*str == '.' && decimalCount == 0)
			{
				// Max of 1 decimal allowed
				decimalCount++;
			}
			else
			{
				return false;
			}
		}
		str++;
	}
	return true;
}

bool Properties::isVariable(const char8* str, char8* outName, size_t outSize)
{
	size_t len = strlen(str);
	if (len > 3 && str[0] == '$' && str[1] == '{' && str[len - 1] == '}')
	{
		size_t size = len - 3;
		if (size > (outSize - 1))
			size = outSize - 1;
		strncpy(outName, str + 2, len - 3);
		outName[len - 3] = 0;
		return true;
	}

	return false;
}

char8 Properties::readChar(File* file)
{
	if (file->IsEndOfFile())
		return -1;

	char8 c = -1;
	if (file->Read(&c, 1) != 1)
		return -1;

	return c;
}

char8* Properties::readLine(File* file, char8* str, int32 num)
{
	if (file == nullptr) return NULL;

	if (num <= 0)
		return NULL;

	char8 c = 0;
	size_t maxCharsToRead = num - 1;
	for (uint32 i = 0; i < maxCharsToRead; ++i)
	{
		uint32 result = file->Read(&c, 1);
		if (result != 1)
		{
			str[i] = '\0';
			break;
		}
		if (c == '\n')
		{
			str[i] = c;
			str[i + 1] = '\0';
			break;
		}
		else if (c == '\r')
		{
			str[i] = c;
			// next may be '\n'
			uint32 pos = file->Tell();

			char8 nextChar = 0;
			if (file->Read(&nextChar, 1) != 1)
			{
				// no more characters
				str[i + 1] = '\0';
				break;
			}
			if (nextChar == '\n')
			{
				if (i == maxCharsToRead - 1)
				{
					str[i + 1] = '\0';
					break;
				}
				else
				{
					str[i + 1] = nextChar;
					str[i + 2] = '\0';
					break;
				}
			}
			else
			{
				file->Seek(pos, EFST_BEGIN);
				str[i + 1] = '\0';
				break;
			}
		}
		str[i] = c;
	}
	return str; // what if first read failed?
}