<?php
// Include database configuration
include 'db_config.php';

// Check if sensor_id parameter is provided
if(isset($_GET['sensor_id'])) {
    // Sanitize and store the sensor_id parameter
    $sensor_id = intval($_GET['sensor_id']);

    // Insert default configuration for the new sensor ID into the database
    $sql = "INSERT INTO configuration (SENSOR_ID, schedule_hour, schedule_minute, sprinkle_decseconds, log_level)
            VALUES ($sensor_id, 8, 30, 20, 1)";
    
    if ($conn->query($sql) === TRUE) {
        echo json_encode(array('success' => true));
    } else {
        echo json_encode(array('success' => false, 'error' => $conn->error));
    }
} else {
    echo json_encode(array('success' => false, 'error' => 'Sensor ID parameter is missing'));
}

// Close database connection
$conn->close();
?>
