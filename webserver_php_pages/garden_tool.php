<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Configure & Log</title>
    <style>
    body {
        font-family: Arial, sans-serif;
        background-color: #f4f4f4;
        margin: 0;
        padding: 0;
    }
    .top-bar {
        background-color: #007bff;
        color: #fff;
        padding: 10px 20px;
        display: flex;
        justify-content: space-between;
    }
    .top-bar a {
        color: #fff;
        text-decoration: none;
        font-weight: bold;
        padding: 5px 10px;
        border-radius: 5px;
        transition: background-color 0.3s;
    }
    .top-bar a:hover {
        background-color: #0056b3;
    }
    .container {
        max-width: 800px;
        margin: 20px auto;
        padding: 20px;
        background-color: #fff;
        border-radius: 8px;
        box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
    }
    h1 {
        text-align: center;
        margin-bottom: 20px;
        color: #333;
    }
    table {
        width: 100%;
        border-collapse: collapse;
        margin-top: 20px;
    }
    th, td {
        padding: 10px;
        text-align: left;
        border-bottom: 1px solid #ccc;
    }
    th {
        background-color: #f2f2f2;
    }
    form {
        display: flex;
        flex-direction: column;
        align-items: center;
    }
    label {
        font-weight: bold;
        margin-bottom: 5px;
    }
    input[type="number"] {
        padding: 10px;
        margin-bottom: 15px;
        border: 1px solid #ccc;
        border-radius: 5px;
        font-size: 16px;
        transition: border-color 0.3s;
        width: 100%;
        box-sizing: border-box;
    }
    input[type="number"]:focus {
        border-color: #007bff;
    }
    button {
        background-color: #007bff;
        color: #fff;
        padding: 10px 20px;
        border: none;
        border-radius: 5px;
        font-size: 16px;
        cursor: pointer;
        transition: background-color 0.3s;
    }
    button:hover {
        background-color: #0056b3;
    }
</style>

</head>
<body>
    <div class="top-bar">
        <a href="#" id="configure-tab" onclick="showSection('configure')">Configure</a>
        <a href="#" id="log-tab" onclick="showSection('log')">Log</a>
        <button onclick="openNewSprinklerPopup()">New Sprinkler</button> <!-- New Sprinkler button -->
        <button onclick="openDeleteSprinklerPopup()">Delete Sprinkler</button> <!-- New Sprinkler button -->
    </div>

    <div class="container" id="configure-section">
        <h1>Configure</h1>
        <form id="configForm" method="POST">
	   
           <label for="sensor_id">Sensor ID:</label>
               <select id="sensor-id-selector" name="sensor_id" onchange="loadConfiguration()">
               <!-- JavaScript will populate this dropdown -->
               </select>
           <br><br>

           <label for="schedule_hour">Schedule Hour (0-23):</label>
           <input type="number" id="schedule_hour" name="schedule_hour" min="0" max="23" required><br><br>
        
           <label for="schedule_minute">Schedule Minute (0-59):</label>
           <input type="number" id="schedule_minute" name="schedule_minute" min="0" max="59" required><br><br>
        
           <label for="sprinkle_decseconds">Sprinkle Decimal Seconds (0-50) (max 5 seconds):</label>
           <input type="number" id="sprinkle_decseconds" name="sprinkle_decseconds" min="0" max="50" required><br><br>

	    <label for="log_level">Log Level:</label>
        	<select id="log_level" name="log_level">
            		<option value="1">1</option>
            		<option value="0">0</option>
        	</select><br><br>

           <button type="button" onclick="saveConfig()">Save</button>
       </form>
       <div id="message"></div> <!-- Message container -->
    </div>
    <script>
        // Function to show confirmation message
        function showMessage(message) {
            document.getElementById("message").innerHTML = message;
        }
    </script>
    <div class="container" id="log-section" style="display: none;">
        <h1>Log</h1>
        <table>
            <thead>
                <tr>
		    <th>Timestamp</th>
                    <th>Sensor ID</th>
                    <th>Sprinkle Action</th>
                    <th>Sprinkle Decseconds</th>
                    <th>Sleep Time</th>
                    <th>Remaining Time</th>
                </tr>
            </thead>
            <tbody>
                <!-- PHP code to fetch and display log data in reverse order goes here -->
	        <?php
		// Include database configuration
		include 'db_config.php';

		// Fetch log data from database
		$sql = "SELECT * FROM sprinkle_log ORDER BY timestamp DESC LIMIT 50";
		$result = $conn->query($sql);

		if ($result->num_rows > 0) {
    		// Output data of each row
    		while($row = $result->fetch_assoc()) {
        		echo "<tr>";
        		echo "<td>" . $row["timestamp"] . "</td>";
			echo "<td>" . $row["SENSOR_ID"] . "</td>";
			echo "<td>" . $row["sprinkle_action"] . "</td>";
        		echo "<td>" . $row["sprinkle_decseconds"] . "</td>";
        		echo "<td>" . $row["sleep_time"] . "</td>";
        		echo "<td>" . $row["remaining_time"] . "</td>";
        		echo "</tr>";
    		}
		} else {
    			echo "<tr><td colspan='5'>No log data available</td></tr>";
		}

		// Close database connection
		$conn->close();
		?>
	    </tbody>
        </table>
    </div>

    <script>
        function showSection(section) {
            if (section === 'configure') {
                document.getElementById('configure-section').style.display = 'block';
                document.getElementById('log-section').style.display = 'none';
                document.getElementById('configure-tab').classList.add('active');
                document.getElementById('log-tab').classList.remove('active');
            } else if (section === 'log') {
                document.getElementById('configure-section').style.display = 'none';
                document.getElementById('log-section').style.display = 'block';
                document.getElementById('configure-tab').classList.remove('active');
                document.getElementById('log-tab').classList.add('active');
            }
	}


    	// Function to populate sensor ID dropdown
    	function populateSensorIds() {
            var sensorIdSelector = document.getElementById("sensor-id-selector");
            sensorIdSelector.innerHTML = ""; // Clear previous options

            // Make an AJAX request to fetch sensor IDs from the server
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "get_sensor_ids.php", true);
            xhr.onreadystatechange = function () {
                if (xhr.readyState == 4 && xhr.status == 200) {
                    var sensorIds = JSON.parse(xhr.responseText);

                    // Add retrieved sensor IDs to the dropdown
                    sensorIds.forEach(function (sensorId) {
                        var option = document.createElement("option");
                        option.text = sensorId;
                        option.value = sensorId;
                        sensorIdSelector.add(option);
                    });
		loadConfiguration();
		}
            };
            xhr.send();
	}

	function saveConfig() {
    	   var formData = new FormData(document.getElementById("configForm"));
    	   var xhr = new XMLHttpRequest();
    	   xhr.onreadystatechange = function() {
           if (xhr.readyState === XMLHttpRequest.DONE) {
	       if (xhr.status === 200) {
                // Success, display success message
                document.getElementById("message").innerHTML = "Configuration updated successfully!";
               } else {
                // Error, display error message
                document.getElementById("message").innerHTML = "Failed to update configuration. Please try again later.";
               }
             }
           };
           xhr.open("POST", "update_config.php", true);
	   xhr.send(formData);
        }

        function loadConfiguration() {
           var sensorId = document.getElementById("sensor-id-selector").value;
           // Make an AJAX request to get configuration based on selected sensor ID
           var xhr = new XMLHttpRequest();
           xhr.open("GET", "get_config.php?sensor_id=" + sensorId, true);
           xhr.onreadystatechange = function () {
	        if (xhr.readyState == 4 && xhr.status == 200) {
                   var configData = JSON.parse(xhr.responseText);
                   // Update page elements with configuration data
		   document.getElementById("schedule_hour").value = configData.schedule_hour;
                   document.getElementById("schedule_minute").value = configData.schedule_minute;
                   document.getElementById("sprinkle_decseconds").value = configData.sprinkle_decseconds;
                   document.getElementById("log_level").value = configData.log_level;
	       }
	   };
           xhr.send();
        }

	window.onload = function() { 
		populateSensorIds(); 
	}


        function openNewSprinklerPopup() {
            var newSensorId = prompt("Enter the new sensor ID:");
             if (newSensorId !== null) { // User clicked OK
                 // Make an AJAX request to add new sprinkler
                 var xhr = new XMLHttpRequest();
                 xhr.open("GET", "add_new_sprinkler.php?sensor_id=" + newSensorId, true);
                 xhr.onreadystatechange = function () {
                    if (xhr.readyState == 4 && xhr.status == 200) {
                        var response = JSON.parse(xhr.responseText);
                        if (response.success) {
                            // If successful, reload sensor IDs and configuration
                            populateSensorIds();
                            loadConfiguration();
                            showMessagePopup("New sprinkler added successfully!");
                        } else {
                            showMessagePopup("Failed to add new sprinkler: " + response.error);
                        }
                    }
                };
                xhr.send();
            }
        }

        function openDeleteSprinklerPopup() {
            var sensorIdToDelete = prompt("Enter the Sprinkler ID to be deleted:");
            if (sensorIdToDelete !== null) { // User clicked OK
                var confirmation = confirm("Are you sure you want to delete sprinkler #" + sensorIdToDelete + "?");
                if (confirmation) {
                    // Make an AJAX request to delete sprinkler
                    var xhr = new XMLHttpRequest();
                    xhr.open("GET", "delete_sprinkler.php?sensor_id=" + sensorIdToDelete, true);
                    xhr.onreadystatechange = function () {
                        if (xhr.readyState == 4 && xhr.status == 200) {
                            var response = JSON.parse(xhr.responseText);
                            if (response.success) {
                                // If successful, reload sensor IDs and configuration
                                populateSensorIds();
                                loadConfiguration();
                                showMessagePopup("Sprinkler deleted successfully!");
                            } else {
                                showMessagePopup("Failed to delete sprinkler: " + response.error);
                            }
                        }
                    };
                    xhr.send();
                } else {
                    showMessagePopup("Deletion canceled by user.");
                }
            }
        }




	</script>

</body>
</html>

