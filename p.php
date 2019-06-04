<?php
function e($s){
	return escapeshellarg($_POST[$s]);
}

function m($msg){
	echo $msg . "<br />\n";
	exit;
}

$myhtpasswd	= "/sbin/myhtpasswd";

$user		= e("user"		);
$domain		= e("domain"		);
$password_old	= e("password_old"	);
$password_new1	= e("password_new1"	);
$password_new2	= e("password_new2"	);

if ($password_new1 != $password_new2)
	m("Passwords does not match!");

$path = "/MAIL/$domain";

system("$myhtpasswd $path $user $password_old $password_new1 >& /dev/null", $r);

if ($r != 0)
	m("Password not changed!");

m("Password changed!");

