<?php
    // Include database configuration
    include 'db_config.php';

    // Set default value for SENSOR_ID if not provided
    $sensor_id = isset($_GET['sensor_id']) ? intval($_GET['sensor_id']) : 1;

    // Retrieve last configuration from database
    $sql = "SELECT schedule_hour, schedule_minute, sprinkle_decseconds, log_level FROM configuration WHERE SENSOR_ID = $sensor_id ORDER BY id DESC LIMIT 1";
    $result = $conn->query($sql);
    if ($result->num_rows > 0) {
       // Fetch associative array
       $row = $result->fetch_assoc();

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
