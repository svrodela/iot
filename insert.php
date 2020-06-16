<?php
include 'bd_controller_local.php';
$t = mysqli_real_escape_string($conn, $_REQUEST['tem']);
$h = mysqli_real_escape_string($conn, $_REQUEST['hum']);
$l = mysqli_real_escape_string($conn, $_REQUEST['luz']);
$u = mysqli_real_escape_string($conn, $_REQUEST['nom']);

if($conn->connect_error) 
{
	die("Error:".$conn->connect_error);
}

$sql = "INSERT INTO registros(IDRegistro, Usuario, Fecha, Tem, Luz, Hum) values (NULL, '$u', now(), '$t', '$l', '$h')";

if ($conn->query($sql)===TRUE)
	{
		echo "agregado correctamente";
	}
else
	{
		echo "Error:".$sql."<br>".$conn->error;
	}
$conn->close();

?>