#!/usr/bin/php-cgi
<?php

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
	$uploads_dir = '../../pages/upload_test/uploads/';
	$filename = basename($_FILES['file']['name']);
	$upload_path = $uploads_dir . $filename;

	// 파일명 중복 방지를 위한 숫자 추가
	$counter = 1;
	while (file_exists($upload_path)) {
		$filename_parts = pathinfo($filename);
		if (isset($filename_parts['extension']))
			$new_filename = $filename_parts['filename'] . '-' . $counter . '.' . $filename_parts['extension'];
		else
			$new_filename = $filename_parts['filename'] . '-' . $counter . $filename_parts['extension'];
		$upload_path = $uploads_dir . $new_filename;
		$counter++;
	}

	if (move_uploaded_file($_FILES['file']['tmp_name'], $upload_path)) {
		echo 'success';
		http_response_code(201);
	} else {
		echo 'fail';
		http_response_code(500);
	}
} else
{
	http_response_code(400);
}


?>