<html>
<head>
 <title>Send Message to all users</title>
</head>
<body>
 <?php
  if(isset($_POST['submit'])){
	include_once 'config.php';
	include_once 'db_functions.php';
	$db = new DB_Functions();
	$users = $db->getAllUsers();
	if ($users != false)
	    $no_of_users = mysql_num_rows($users);
	else
	    $no_of_users = 0;

	$registatoin_ids = array();

   	while($row = mysql_fetch_array($users)){
    		array_push($registatoin_ids, $row['gcm_regid']);
  	 }
 
   	// Set POST variables
	$url = 'https://android.googleapis.com/gcm/send';
   
    	$message = array("message" => $_POST['message']);
	$fields = array(
	     'registration_ids' => $registatoin_ids,
	     'data' => $message,
	 );
   
	$headers = array(
	     'Authorization: key='. GOOGLE_API_KEY,
	     'Content-Type: application/json'
	 );
	 // Open connection
	 $ch = curl_init();
   
	 // Set the url, number of POST vars, POST data
	 curl_setopt($ch, CURLOPT_URL, $url);
   
	 curl_setopt($ch, CURLOPT_POST, true);
	 curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
	 curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
   
	 // Disabling SSL Certificate support temporarly
	 curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
   
	 curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($fields));
   
	 // Execute post
	 $result = curl_exec($ch);
	 if ($result === FALSE) {
	     die('Curl failed: ' . curl_error($ch));
	 }
   
	 // Close connection
	 curl_close($ch);
	 echo $result;
    }
 ?>
 <form method="post" action="gcm_msg_all.php">
  <font size="5">
  <label>Insert Message: </label><input type="text" name="message" />
  <input type="submit" name="submit" value="Send" />
  </font>
 </form>
</body>
</html>
