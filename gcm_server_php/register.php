<?php
 
// response json
$json = array();
 
/**
 * Registering a user device
 * Store reg id in users table
 */
if (isset($_POST["name"]) && isset($_POST["email"]) && isset($_POST["regId"])) {
    $name = $_POST["name"];
    $email = $_POST["email"];
    $gcm_regid = $_POST["regId"]; // GCM Registration ID
    // Store user details in db
    include_once './db_functions.php';
    include_once './GCM.php';
 
    $db = new DB_Functions();
    $gcm = new GCM();

    if ($db->checkUser($gcm_regid) == true){
	$message = array("message" => "Your device was previously registered.");
	$registatoin_ids = array($gcm_regid);
    	$result = $gcm->send_notification($registatoin_ids, $message);
 	echo $result;
    } else {
    	$res = $db->storeUser($name, $email, $gcm_regid);
 	if ($res != false){
        	$registatoin_ids = array($gcm_regid);
    		$message = array("message" => "Your device has been registered.");
    		$result = $gcm->send_notification($registatoin_ids, $message);
 		echo $result;
	}
    }
} else {
    // user details missing
}
?>
