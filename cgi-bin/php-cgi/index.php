#!/usr/bin/php-cgi
<?php
session_start();

if(!isset($_SESSION['username'])) {
  echo "<script>location.replace('login.php');</script>";            
  $username = 'not_available';
  $name = 'not_available';
}
else {
    $username = $_SESSION['username'];
    $name = $_SESSION['name'];
} 
?>


<!DOCTYPE html>
<html lang="ko">
<head>
  <meta charset="utf-8">
  <title>My Page</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      /* background-color: #3259a6; */
      color: #3259a6;
    }
    .base {
      display: flex;
      flex-direction: column;
      /* justify-content: center; */
      align-items: center;
      height: 100vh;
    }
    h2 {
      text-align: center;
      margin: 20px 0;
    }
    .btn {
      display: block;
      width: 150px;
      padding: 10px;
      background-color: #3259a6;
      color: white;
      border: none;
      border-radius: 5px;
      font-size: 1.1em;
      cursor: pointer;
    }
  </style>
</head>
<body>
  <div class="base">
    <h2><?php echo "Hi, $username($name)";?></h2>
    <button type="button" class="btn" onclick="location.href='logout.php'">
      LOGOUT
    </button>
  </div>
</body>
</html>
