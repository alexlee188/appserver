<html>
<head>
	<title>Insert New Customer></title>
</head>
<body>
<h3>If you have already registered as an SGnurse customer, just call SGnurse admin at 97851440 for booking a home nurse visit.  If you are new, please register below.  After registration, SGnurse admin will call you to confirm.</h3>
<?php
	if(isset($_POST['submit'])){
	require_once 'config.php';
	$conn = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_DATABASE, DB_PORT);
	if (mysqli_connect_errno())
	  {
	  echo "Failed to connect to MySQL: " . mysqli_connect_error();
	  }
 	$result = mysqli_query($conn, "insert into CUSTOMER (NAME_1,NAME_2,ADDR_BLK_NO,
		ADDR_STREET_1,ADDR_STREET_2,ADDR_POSTCODE,PHONE,MOBILE) VALUES (
		'$_POST[NAME_1]','$_POST[NAME_2]','$_POST[ADDR_BLK_NO]','$_POST[ADDR_STREET_1]',
		'$_POST[ADDR_STREET_2]','$_POST[ADDR_POSTCODE]','$_POST[PHONE]','$_POST[MOBILE]')");
	echo "1 record added";
	mysqli_close($conn);
	}
?>

<form action="Booking.php" method="post">
Name of Patient: <input type="text" name="NAME_1" max_width="40" size="40" /><br><br>
Name of Carer: <input type="text" name="NAME_2" max_width="40" size="40" /><br><br>
Block No./House No.: <input type="text" name="ADDR_BLK_NO" max_width="10" size="10" /><br><br>
Street (line 1): <input type="text" name="ADDR_STREET_1" max_width="80" size="80" /><br><br>
Street (line 2): <input type="text" name="ADDR_STREET_1" max_width="80" size="80" /><br><br>
Post Code: <input type="text" name="ADDR_POSTCODE" max_width="10" size="10" /><br><br>
Phone No.: <input type="text" name="PHONE" max_width="16" size="16" /><br><br>
Mobile No.: <input type="text" name="MOBILE" max_width="16" size="16" /><br><br>
<input type="submit" />
</form>
</body>
</html>
