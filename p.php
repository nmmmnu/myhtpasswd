<?php
function e($s){
	return escapeshellarg($_POST[$s]);
}

function m($msg){
	echo $msg . "\n";
	exit;
}

$myhtpasswd	= "/VMAIL/myhtpasswd";

$user		= e("user"		);
$domain		= e("domain"		);
$password_old	= e("password_old"	);
$password_new1	= e("password_new1"	);
$password_new2	= e("password_new2"	);

if ($password_new1 != $password_new2)
	m("Passwords does not match!");

$path = "/VMAI/AUTH/$domain";

$r = system("$myhtpasswd, $path, $password_old, $password_new1);

if ($r != 0)
	m("Password not changed!");

m("Password changed!");

