<?php
// Include database configuration
include 'db_config.php';

// Check if sensor_id parameter is provided
if(isset($_GET['sensor_id'])) {
    // Sanitize and store the sensor_id parameter
    $sensor_id = intval($_GET['sensor_id']);

    // Delete all configuration lines associated with the specified sensor ID
    $sql = "DELETE FROM configuration WHERE SENSOR_ID = ?";
    
    // Prepare and bind SQL statement
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("i", $sensor_id);
    
    // Execute statement
    if ($stmt->execute()) {
        // Send success response
        echo json_encode(array('success' => true));
    } else {
        // Send error response
        echo json_encode(array('success' => false, 'error' => $conn->error));
    }

    // Close statement
    $stmt->close();
} else {
    // Send error response if sensor_id parameter is missing
    echo json_encode(array('success' => false, 'error' => 'Sensor ID parameter is missing'));
}

// Close database connection
$conn->close();
?>
