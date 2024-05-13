<?php
// Include database configuration
include 'db_config.php';

// Set default value for SENSOR_ID if not provided
$sensor_id = isset($_GET['SENSOR_ID']) ? intval($_GET['SENSOR_ID']) : 1;


// Get the local timezone offset in seconds
$timezone_offset_seconds = intval(date('Z'));

// SQL query to retrieve data from the configuration table
$sql = "SELECT sensor_id, log_level, schedule_hour, schedule_minute, sprinkle_decseconds FROM configuration WHERE SENSOR_ID = $sensor_id ORDER BY id DESC LIMIT 1";

$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // Fetch associative array
    $row = $result->fetch_assoc();
    
    // Add timezone offset to the row data
    $row['timezone_offset'] = "$timezone_offset_seconds";

    // Convert fields to a single JSON object
    $jsonObject = json_encode($row);

    // Set content type to JSON
    header('Content-Type: application/json');

    // Output JSON object
    echo $jsonObject;
} else {
    echo json_encode(array('error' => 'No data found'));
}

// Close database connection
$conn->close();
?>
