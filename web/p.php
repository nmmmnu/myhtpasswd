<?php
require_once __DIR__ . "/checks.php";

function m($msg){
	echo $msg . "<br />\n";
	exit;
}

function e($s, $min){
	$val = $_POST[$s];

	if (strlen($val) < $min)
		m("Parameter too short");

	return escapeshellarg($val);
}

$myhtpasswd	= "/usr/local/sbin/myhtpasswd";

$user		= e("user"		, 1); // u@x.de
$domain		= e("domain"		, 4);
$password_old	= e("password_old"	, 1);
$password_new1	= e("password_new1"	, 6);
$password_new2	= e("password_new2"	, 6);

if ($password_new1 != $password_new2)
	m("Passwords does not match!");

system("$myhtpasswd $domain $user $password_old $password_new1 >& /tmp/myhtpasswd.web", $r);

if ($r != 0)
	m("Password not changed!");

m("Password changed!");

