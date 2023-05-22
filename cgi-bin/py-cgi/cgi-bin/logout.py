#!/usr/bin/env python3

# HTTP header
print("Set-cookie: session_id=_")
print("Set-cookie: vip_level=_")
print("Content-type: text/html\n")

# Redirect to the login page
print("<script>location.replace('../login.html');</script>")