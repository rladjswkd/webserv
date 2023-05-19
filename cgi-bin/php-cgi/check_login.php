#!/usr/bin/php-cgi
<!DOCTYPE html>
<html>
<head>
   <meta charset="utf-8">
   <title></title>
</head>
<body>
   <?php
   session_start();

      $username = $_POST['id'];
      $userpass = $_POST['pw'];
      
      //결과가 존재하면 세션 생성
      if ($username == 'webserv' && $userpass == 'no') {
         $_SESSION['username'] = $username;
         $_SESSION['name'] = 'Jacob';
         // setcookie("session_id",'webserv-Jacob');
         echo "<script>location.replace('index.php');</script>";
         exit;
      }
      else if ($username == 'irc' && $userpass == 'yes') {
         $_SESSION['username'] = $username;
         $_SESSION['name'] = 'Daniel';
         // setcookie("session_id",'irc-Daniel');
         echo "<script>location.replace('index.php');</script>";
         exit;
      }
      else {
         echo "<script>alert('Invalid username or password')</script>";
         echo "<script>location.replace('login.php');</script>";
         exit;
      }
      ?>
   </body>