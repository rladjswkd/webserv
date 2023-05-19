#!/bin/sh
if [ ! -d "./pages" ];then
  mkdir -p ./pages/upload_test/uploads/ ./pages/py_cgitest/
	cp ./cgi-bin/upload/upload_delete.html ./pages/upload_test/upload_delete.html
	cp ./cgi-bin/py-cgi/login.html ./pages/py_cgitest/login.html
	cp ./cgi-bin/main_page.html ./pages/main_page.html
	cp -r ./cgi-bin/sample/ ./pages/
	cp -r ./cgi-bin/directoryList/ ./pages/
	echo "test1" > ./pages/upload_test/uploads/test1
	echo "test2" > ./pages/upload_test/uploads/test2
fi