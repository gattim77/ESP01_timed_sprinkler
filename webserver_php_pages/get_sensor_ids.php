<?php
// Include database configuration
include 'db_config.php';

// SQL query to retrieve distinct sensor IDs
$sql = "SELECT DISTINCT SENSOR_ID FROM configuration";

$result = $conn->query($sql);

$sensorIds = array();

if ($result->num_rows > 0) {
    // Fetch associative array
    while($row = $result->fetch_assoc()) {
        $sensorIds[] = $row['SENSOR_ID'];
    }
}

// Set content type to JSON
header('Content-Type: application/json');

// Output JSON array of sensor IDs
echo json_encode($sensorIds);

// Close database connection
$conn->close();
?>
