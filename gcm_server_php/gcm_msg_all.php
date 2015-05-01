<html>
<head>
 <title>Send Message to all users</title>
</head>
<body>
 <?php
  if(isset($_POST['submit'])){
        require_once 'config.php';
        // connecting to mysql
        $conn = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_DATABASE, DB_PORT);
        $result = mysqli_query($conn, "select * FROM gcm_users");
        $no_of_users = mysqli_num_rows($result);

	$registatoin_ids = array();

   	while($row = mysqli_fetch_assoc($result)){
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
	 // curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
	 curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 2);
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
