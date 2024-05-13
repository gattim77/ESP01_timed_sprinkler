<?php
include 'db_config.php';

// Get data from the HTTP request
$data = json_decode(file_get_contents('php://input'), true);

// Extract values from the received data
$sensor_id = $data['sensor_id'];
$sprinkle_action= $data['sprinkle_action'];
$sprinkle_decseconds = $data['sprinkle_decseconds'];
$sleep_time = $data['sleep_time'];
$remaining_time = $data['remaining_time'];

// Prepare SQL statement to insert data into the table
$sql = "INSERT INTO sprinkle_log (SENSOR_ID, sprinkle_action,sprinkle_decseconds,sleep_time, remaining_time) VALUES ('$sensor_id', '$sprinkle_action','$sprinkle_decseconds','$sleep_time','$remaining_time')";

// Execute SQL statement
if ($conn->query($sql) === TRUE) {
    echo "Data inserted successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

// Close database connection
$conn->close();

?>
