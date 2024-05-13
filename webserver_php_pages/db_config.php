<?php
// Database configuration
$db_host = "localhost";
$db_username = "garden";
$db_password = "garden";
$db_name = "garden_log1";

// Create database connection
$conn = new mysqli($db_host, $db_username, $db_password, $db_name);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
?>
