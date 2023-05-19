#!/usr/bin/env python3
import os

# 세션 ID를 가져옵니다.
cookie = os.environ.get('HTTP_COOKIE')
session_id = ''

try:
    for item in cookie.split(';'):
        key, value = item.strip().split('=')
        if key == 'session_id':
            session_id = value

    if session_id == 'webserv-Jacob' or session_id == 'irc-David':
        if session_id == 'webserv-Jacob':
            user_id = 'webserv'
            user_name = 'Jacob'
            vip_level = 1
        elif session_id == 'irc-David':
            user_id = 'irc'
            user_name = 'David'
            vip_level = 2
        
        printHttp = f'''
<!DOCTYPE html>
<html lang="ko">
<head>
<meta charset="utf-8">
<title>My Page</title>
<style>
    body {{
    font-family: Arial, sans-serif;
    margin: 0;
    padding: 0;
    
    color: #d63f63;
    }}
    .base {{
    display: flex;
    flex-direction: column;
    
    align-items: center;
    height: 100vh;
    }}
    h2 {{
    text-align: center;
    margin: 20px 0;
    }}
    .btn {{
    display: block;
    width: 150px;
    padding: 10px;
    background-color: #d63f63;
    color: white;
    border: none;
    border-radius: 5px;
    font-size: 1.1em;
    cursor: pointer;
    }}
</style>
</head>
<body>
<div class="base">
    <h2>Hi, {user_id}({user_name}) : VIP LEVEL({vip_level})</h2>
    <button type="button" class="btn" onclick="location.href='logout.py'">
    LOGOUT
    </button>
</div>
</body>
</html>
                '''
        print(printHttp)
    else:
        print("<script>location.replace('../login.html');</script>")
except:
    print("<script>location.replace('../login.html');</script>")