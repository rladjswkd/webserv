#!/usr/bin/env python3
import cgi
from datetime import datetime, timedelta

def set_cookie(cookie_name, cookie_value, expires=False):
    expires_time = datetime.now() + timedelta(hours=1)
    cookie = "Set-cookie: " + cookie_name + "=" + cookie_value + ";"
    if expires:
        cookie += " Expires=" + expires_time.strftime('%a, %d %b %Y %H:%M:%S GMT')
    return cookie

def body_upper():
    body = "<!DOCTYPE html> <html> <head> <meta charset=""utf-8""> <title> </title> </head> <body>"
    print(body)


# Get the form data
form = cgi.FieldStorage()
username = form.getvalue("id")
password = form.getvalue("pw")

# Check if the username and password are correct
if username == "webserv" and password == "no":
    print(set_cookie("session_id", username + "-Jacob"))
    print("")
    body_upper()
    print("<script>location.replace('index.py');</script>")
    print("</body>")
elif username == "irc" and password == "yes":
    print(set_cookie("session_id", username + "-David"))
    print("")
    body_upper()
    print("<script>location.replace('index.py');</script>")
    print("</body>")
else:    
    body_upper()
    print("<script>alert('Invalid username or password')</script>")
    print("<script>location.replace('../login.html');</script>")
    print("</body>")

