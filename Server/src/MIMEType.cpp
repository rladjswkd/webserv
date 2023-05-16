#include "MIMEType.hpp"
#include <utility>

MIMEType::MIMEMap MIMEType::createMimeMap()
{
	MIMEMap	ret;	
	
	ret.insert(std::make_pair("html", "text/html"));
	ret.insert(std::make_pair("htm", "text/html"));
	ret.insert(std::make_pair("shtml", "text/html"));
	ret.insert(std::make_pair("css", "text/css"));
	ret.insert(std::make_pair("xml", "text/xml"));
	ret.insert(std::make_pair("gif", "image/gif"));
	ret.insert(std::make_pair("jpeg", "image/jpeg"));
	ret.insert(std::make_pair("jpg", "image/jpeg"));
	ret.insert(std::make_pair("js", "application/javascript"));
	ret.insert(std::make_pair("atom", "application/atom+xml"));
	ret.insert(std::make_pair("rss", "application/rss+xml"));
	ret.insert(std::make_pair("mml", "text/mathml"));
	ret.insert(std::make_pair("txt", "text/plain"));
	ret.insert(std::make_pair("jad", "text/vnd.sun.j2me.app-descriptor"));
	ret.insert(std::make_pair("wml", "text/vnd.wap.wml"));
	ret.insert(std::make_pair("htc", "text/x-component"));
	ret.insert(std::make_pair("avif", "image/avif"));
	ret.insert(std::make_pair("png", "image/png"));
	ret.insert(std::make_pair("svg", "image/svg+xml"));
	ret.insert(std::make_pair("svgz", "image/svg+xml"));
	ret.insert(std::make_pair("tif", "image/tiff"));
	ret.insert(std::make_pair("tiff", "image/tiff"));
	ret.insert(std::make_pair("wbmp", "image/vnd.wap.wbmp"));
	ret.insert(std::make_pair("webp", "image/webp"));
	ret.insert(std::make_pair("ico", "image/x-icon"));
	ret.insert(std::make_pair("jng", "image/x-jng"));
	ret.insert(std::make_pair("bmp", "image/x-ms-bmp"));
	ret.insert(std::make_pair("woff", "font/woff"));
	ret.insert(std::make_pair("woff2", "font/woff2"));
	ret.insert(std::make_pair("jar", "application/java-archive"));
	ret.insert(std::make_pair("war", "application/java-archive"));
	ret.insert(std::make_pair("ear", "application/java-archive"));
	ret.insert(std::make_pair("json", "application/json"));
	ret.insert(std::make_pair("hqx", "application/mac-binhex40"));
	ret.insert(std::make_pair("doc", "application/msword"));
	ret.insert(std::make_pair("pdf", "application/pdf"));
	ret.insert(std::make_pair("ps", "application/postscript"));
	ret.insert(std::make_pair("eps", "application/postscript"));
	ret.insert(std::make_pair("ai", "application/postscript"));
	ret.insert(std::make_pair("rtf", "application/rtf"));
	ret.insert(std::make_pair("m3u8", "application/vnd.apple.mpegurl"));
	ret.insert(std::make_pair("kml", "application/vnd.google-earth.kml+xml"));
	ret.insert(std::make_pair("kmz", "application/vnd.google-earth.kmz"));
	ret.insert(std::make_pair("xls", "application/vnd.ms-excel"));
	ret.insert(std::make_pair("eot", "application/vnd.ms-fontobject"));
	ret.insert(std::make_pair("ppt", "application/vnd.ms-powerpoint"));
	ret.insert(std::make_pair("odg", "application/vnd.oasis.opendocument.graphics"));
	ret.insert(std::make_pair("odp", "application/vnd.oasis.opendocument.presentation"));
	ret.insert(std::make_pair("ods", "application/vnd.oasis.opendocument.spreadsheet"));
	ret.insert(std::make_pair("odt", "application/vnd.oasis.opendocument.text"));
	ret.insert(std::make_pair("pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"));
	ret.insert(std::make_pair("xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
	ret.insert(std::make_pair("docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
	ret.insert(std::make_pair("wmlc", "application/vnd.wap.wmlc"));
	ret.insert(std::make_pair("wasm", "application/wasm"));
	ret.insert(std::make_pair("7z", "application/x-7z-compressed"));
	ret.insert(std::make_pair("cco", "application/x-cocoa"));
	ret.insert(std::make_pair("jardiff", "application/x-java-archive-diff"));
	ret.insert(std::make_pair("jnlp", "application/x-java-jnlp-file"));
	ret.insert(std::make_pair("run", "application/x-makeself"));
	ret.insert(std::make_pair("pl", "application/x-perl"));
	ret.insert(std::make_pair("pm", "application/x-perl"));
	ret.insert(std::make_pair("prc", "application/x-pilot"));
	ret.insert(std::make_pair("pdb", "application/x-pilot"));
	ret.insert(std::make_pair("rar", "application/x-rar-compressed"));
	ret.insert(std::make_pair("rpm", "application/x-redhat-package-manager"));
	ret.insert(std::make_pair("sea", "application/x-sea"));
	ret.insert(std::make_pair("swf", "application/x-shockwave-flash"));
	ret.insert(std::make_pair("sit", "application/x-stuffit"));
	ret.insert(std::make_pair("tcl", "application/x-tcl"));
	ret.insert(std::make_pair("tk", "application/x-tcl"));
	ret.insert(std::make_pair("der", "application/x-x509-ca-cert"));
	ret.insert(std::make_pair("pem", "application/x-x509-ca-cert"));
	ret.insert(std::make_pair("crt", "application/x-x509-ca-cert"));
	ret.insert(std::make_pair("xpi", "application/x-xpinstall"));
	ret.insert(std::make_pair("xhtml", "application/xhtml+xml"));
	ret.insert(std::make_pair("xspf", "application/xspf+xml"));
	ret.insert(std::make_pair("zip", "application/zip"));
	ret.insert(std::make_pair("bin", "application/octet-stream"));
	ret.insert(std::make_pair("exe", "application/octet-stream"));
	ret.insert(std::make_pair("dll", "application/octet-stream"));
	ret.insert(std::make_pair("deb", "application/octet-stream"));
	ret.insert(std::make_pair("dmg", "application/octet-stream"));
	ret.insert(std::make_pair("iso", "application/octet-stream"));
	ret.insert(std::make_pair("img", "application/octet-stream"));
	ret.insert(std::make_pair("msi", "application/octet-stream"));
	ret.insert(std::make_pair("msp", "application/octet-stream"));
	ret.insert(std::make_pair("msm", "application/octet-stream"));
	ret.insert(std::make_pair("mid", "audio/midi"));
	ret.insert(std::make_pair("midi", "audio/midi"));
	ret.insert(std::make_pair("kar", "audio/midi"));
	ret.insert(std::make_pair("mp3", "audio/mpeg"));
	ret.insert(std::make_pair("ogg", "audio/ogg"));
	ret.insert(std::make_pair("m4a", "audio/x-m4a"));
	ret.insert(std::make_pair("ra", "audio/x-realaudio"));
	ret.insert(std::make_pair("3gpp", "video/3gpp"));
	ret.insert(std::make_pair("3gp", "video/3gpp"));
	ret.insert(std::make_pair("ts", "video/mp2t"));
	ret.insert(std::make_pair("mp4", "video/mp4"));
	ret.insert(std::make_pair("mpeg", "video/mpeg"));
	ret.insert(std::make_pair("mpg", "video/mpeg"));
	ret.insert(std::make_pair("mov", "video/quicktime"));
	ret.insert(std::make_pair("webm", "video/webm"));
	ret.insert(std::make_pair("flv", "video/x-flv"));
	ret.insert(std::make_pair("m4v", "video/x-m4v"));
	ret.insert(std::make_pair("mng", "video/x-mng"));
	ret.insert(std::make_pair("asx", "video/x-ms-asf"));
	ret.insert(std::make_pair("asf", "video/x-ms-asf"));
	ret.insert(std::make_pair("wmv", "video/x-ms-wmv"));
	ret.insert(std::make_pair("avi", "video/x-msvideo"));

	return (ret);
}

const MIMEType::MIMEMap & MIMEType::getInstance()
{
	static MIMEMap	mimeMap = this->createMimeMap();

	return (mimeMap);
}

