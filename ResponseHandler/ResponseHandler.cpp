#include "ResponseHandler.hpp"

Response ResponseHandler::response;
ResponseHandler::ResponseMessageType ResponseHandler::outputResponseMessage;


ResponseHandler::ResponseMessageType ResponseHandler::createResponseMessage(Response &inputResponseMessage)
{
    response = inputResponseMessage;

    
    return outputResponseMessage; 
}