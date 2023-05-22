#!/usr/bin/php-cgi
<?php
session_start();
session_destroy();
setcookie("session_id",'');
?>
<script>    
    alert("You've been logged out");    
    location.replace('index.php');
</script>