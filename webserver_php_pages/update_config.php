<?php
// Include database configuration
include 'db_config.php';

// Check if form is submitted
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $sensor_id = intval($_POST["sensor_id"]);
    $schedule_hour = intval($_POST["schedule_hour"]);
    $schedule_minute = intval($_POST["schedule_minute"]);
    $sprinkle_decseconds = intval($_POST["sprinkle_decseconds"]);
    $log_level = intval($_POST["log_level"]);

    // Prepare and bind SQL statement
    $stmt = $conn->prepare("INSERT INTO configuration (SENSOR_ID, schedule_hour, schedule_minute, sprinkle_decseconds, LOG_LEVEL) VALUES (?, ?, ?, ?, ?)");
    $stmt->bind_param("iiiii", $sensor_id, $schedule_hour, $schedule_minute, $sprinkle_decseconds, $log_level);
 
    // Execute query
    if ($stmt->execute()) {
        // Show confirmation message
        echo '<script type="text/javascript">window.onload = function() { showMessage("Configuration saved successfully"); }</script>';
    } else {
        // Show error message
        echo '<script type="text/javascript">window.onload = function() { showMessage("Error saving configuration"); }</script>';
    }

    $stmt->close();

    // Redirect back to configure.php after 3 seconds
    echo '<meta http-equiv="refresh" content="3;url=garden_tool.php">';

}
?>
