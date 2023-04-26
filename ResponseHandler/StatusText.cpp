#include "StatusText.hpp"

StatusText::StatusText()
{
  statusTextMap["100"] = "Continue";
  statusTextMap["101"] = "Switching Protocols";
  statusTextMap["102"] = "Processing";
  statusTextMap["103"] = "Early Hints";
  statusTextMap["200"] = "OK";
  statusTextMap["201"] = "Created";
  statusTextMap["202"] = "Accepted";
  statusTextMap["203"] = "Non-Authoritative Information";
  statusTextMap["204"] = "No Content";
  statusTextMap["205"] = "Reset Content";
  statusTextMap["206"] = "Partial Content";
  statusTextMap["207"] = "Multi-Status";
  statusTextMap["208"] = "Already Reported";
  statusTextMap["226"] = "IM Used";
  statusTextMap["300"] = "Multiple Choices";
  statusTextMap["301"] = "Moved Permanently";
  statusTextMap["302"] = "Found";
  statusTextMap["303"] = "See Other";
  statusTextMap["304"] = "Not Modified";
  statusTextMap["305"] = "Use Proxy";
  statusTextMap["306"] = "(Unused)";
  statusTextMap["307"] = "Temporary Redirect";
  statusTextMap["308"] = "Permanent Redirect";
  statusTextMap["400"] = "Bad Request";
  statusTextMap["401"] = "Unauthorized";
  statusTextMap["402"] = "Payment Required";
  statusTextMap["403"] = "Forbidden";
  statusTextMap["404"] = "Not Found";
  statusTextMap["405"] = "Method Not Allowed";
  statusTextMap["406"] = "Not Acceptable";
  statusTextMap["407"] = "Proxy Authentication Required";
  statusTextMap["408"] = "Request Timeout";
  statusTextMap["409"] = "Conflict";
  statusTextMap["410"] = "Gone";
  statusTextMap["411"] = "Length Required";
  statusTextMap["412"] = "Precondition Failed";
  statusTextMap["413"] = "Payload Too Large";
  statusTextMap["414"] = "URI Too Long";
  statusTextMap["415"] = "Unsupported Media Type";
  statusTextMap["416"] = "Range Not Satisfiable";
  statusTextMap["417"] = "Expectation Failed";
  statusTextMap["418"] = "I'm a teapot";
  statusTextMap["421"] = "Misdirected Request";
  statusTextMap["422"] = "Unprocessable Entity";
  statusTextMap["423"] = "Locked";
  statusTextMap["424"] = "Failed Dependency";
  statusTextMap["425"] = "Too Early";
  statusTextMap["426"] = "Upgrade Required";
  statusTextMap["428"] = "Precondition Required";
  statusTextMap["429"] = "Too Many Requests";
  statusTextMap["431"] = "Request Header Fields Too Large";
  statusTextMap["451"] = "Unavailable For Legal Reasons";
  statusTextMap["500"] = "Internal Server Error";
  statusTextMap["501"] = "Not Implemented";
  statusTextMap["502"] = "Bad Gateway";
  statusTextMap["503"] = "Service Unavailable";
  statusTextMap["504"] = "Gateway Timeout";
  statusTextMap["505"] = "HTTP Version Not Supported";
  statusTextMap["506"] = "Variant Also Negotiates";
  statusTextMap["507"] = "Insufficient Storage";
  statusTextMap["508"] = "Loop Detected";
  statusTextMap["509"] = "Bandwidth Limit Exceeded";
  statusTextMap["510"] = "Not Extended";
  statusTextMap["511"] = "Network Authentication Required";
};


StatusText::StatusTextMapType StatusText::getStatusTextMap() const
{
    return statusTextMap;
}