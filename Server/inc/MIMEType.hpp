#ifndef MIMETYPE_HPP
# define MIMETYPE_HPP

# include <map>
# include <string>

class MIMEType
{
public:
	typedef const std::string					Extension, ContentType;
	typedef std::map<Extension, ContentType>	MIMEMap;

private:
	MIMEType();
	static MIMEMap	createMimeMap();

public:
	static const MIMEMap	&getInstance();
};
#endif