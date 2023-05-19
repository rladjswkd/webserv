#!/usr/bin/php-cgi
<?php
session_start();

$_SESSION['username'] = 'webserv';
$_SESSION['userpass'] = 'no';
$_SESSION['name'] = 'Jacob';

echo "<script>location.replace('index.php');</script>";            

?>