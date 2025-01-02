#include <ESP8266WiFi.h>  // Use ESP32WiFi.h if using ESP32
#include <ESP8266WebServer.h>  // Use ESP32WebServer.h if using ESP32

// Motor Pins connected to L298N Motor Driver
const int motor1Pin1 = D1;  // IN1 (Motor 1)
const int motor1Pin2 = D2;  // IN2 (Motor 1)
const int motor2Pin1 = D5;  // IN3 (Motor 2)
const int motor2Pin2 = D6;  // IN4 (Motor 2)

// Wi-Fi credentials
const char* ssid = "The Millennium School";  // Replace with your Wi-Fi name
const char* password = "TM$12345";           // Replace with your Wi-Fi password

// Create a web server on port 80
ESP8266WebServer server(80);

// Variable to track control mode
bool joystickControl = false;  // false for Web Control, true for Joystick Control

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);

  // Set motor control pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Print the IP address of the ESP8266
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Serve the HTML page
  server.on("/", HTTP_GET, handleRoot);

  // Define the routes for controlling the car
  server.on("/forward", HTTP_GET, moveForward);
  server.on("/backward", HTTP_GET, moveBackward);
  server.on("/left", HTTP_GET, turnLeft);
  server.on("/right", HTTP_GET, turnRight);
  server.on("/stop", HTTP_GET, stopMotors);
  server.on("/switchToJoystick", HTTP_GET, switchToJoystick);
  server.on("/switchOffJoystick", HTTP_GET, switchOffJoystick);

  // Start the server
  server.begin();
}

void loop() {
  // Handle incoming client requests
  server.handleClient();
}

// Serve the HTML page with the "Switch to Joystick Control" button and CSS
void handleRoot() {
  String html = "<html><head>";
  html += "<style>";

  // Gaming theme CSS
  html += "body {";
  html += "  font-family: 'Arial', sans-serif;";
  html += "  background: #111;";
  html += "  color: #fff;";
  html += "  text-align: center;";
  html += "  padding: 20px;";
  html += "  margin: 0;";
  html += "  font-size: 18px;";
  html += "}";

  // Heading style
  html += "h1 {";
  html += "  font-size: 50px;";
  html += "  color: #f39c12;";
  html += "  text-shadow: 2px 2px 10px rgba(255, 255, 255, 0.3);";
  html += "}";

  // Buttons style - Gaming controller look
  html += "button {";
  html += "  background: #1abc9c;";
  html += "  border: none;";
  html += "  color: #fff;";
  html += "  font-size: 22px;";
  html += "  width: 200px;";
  html += "  padding: 15px;";
  html += "  margin: 15px;";
  html += "  border-radius: 50px;";
  html += "  transition: background 0.3s, transform 0.2s;";
  html += "  cursor: pointer;";
  html += "  box-shadow: 0 4px 15px rgba(0, 0, 0, 0.3);";
  html += "  text-transform: uppercase;";
  html += "  letter-spacing: 2px;";
  html += "}";

  // Hover effect for buttons
  html += "button:hover {";
  html += "  background: #16a085;";
  html += "  transform: scale(1.1);";
  html += "}";

  // Active button effect
  html += "button:active {";
  html += "  background: #1e7c61;";
  html += "  transform: scale(0.95);";
  html += "}";

  // Switch Button - different color
  html += ".switch-btn {";
  html += "  background: #e74c3c;";
  html += "  color: #fff;";
  html += "}";

  // Switch button hover effect
  html += ".switch-btn:hover {";
  html += "  background: #c0392b;";
  html += "}";

  // Status message for joystick mode
  html += ".status {";
  html += "  font-size: 20px;";
  html += "  margin-top: 20px;";
  html += "  color: #f39c12;";
  html += "  text-shadow: 1px 1px 5px rgba(0, 0, 0, 0.5);";
  html += "}";

  // Toggle Switch CSS
  html += ".switch-container {";
  html += "  display: inline-block;";
  html += "  margin-top: 20px;";
  html += "}";

  html += ".switch {";
  html += "  position: relative;";
  html += "  display: inline-block;";
  html += "  width: 100px;";
  html += "  height: 50px;";
  html += "}";

  html += ".switch input {";
  html += "  opacity: 0;";
  html += "  width: 0;";
  html += "  height: 0;";
  html += "}";

  html += ".slider {";
  html += "  position: absolute;";
  html += "  cursor: pointer;";
  html += "  top: 0;";
  html += "  left: 0;";
  html += "  right: 0;";
  html += "  bottom: 0;";
  html += "  background-color: #ccc;";
  html += "  transition: 0.4s;";
  html += "  border-radius: 50px;";
  html += "}";

  html += ".slider:before {";
  html += "  position: absolute;";
  html += "  content: \"\";";
  html += "  height: 40px;";
  html += "  width: 40px;";
  html += "  border-radius: 50px;";
  html += "  left: 5px;";
  html += "  bottom: 5px;";
  html += "  background-color: white;";
  html += "  transition: 0.4s;";
  html += "}";

  html += "input:checked + .slider {";
  html += "  background-color: #4CAF50;";
  html += "}";

  html += "input:checked + .slider:before {";
  html += "  transform: translateX(50px);";
  html += "}";

  html += "</style>";
  html += "</head><body>";

  html += "<h1>RC Car Control</h1>";

  if (joystickControl) {
    html += "<p class='status'>Your controller is connected</p>";
  } else {
    html += "<p class='status'>Your joystick is disconnected</p>";
    html += "<button id='forward'>Move Forward</button><br>";
    html += "<button id='backward'>Move Backward</button><br>";
    html += "<button id='left'>Turn Left</button><br>";
    html += "<button id='right'>Turn Right</button><br>";
    html += "<button id='stop'>Stop</button><br>";
  }

  // On/Off toggle switch for joystick control
  html += "<div class='switch-container'>";
  html += "<label class='switch'>";
  html += "<input type='checkbox' id='joystickSwitch' onchange='toggleJoystickControl()'>";
  html += "<span class='slider'></span>";
  html += "</label>";
  html += "</div>";

  html += "<script>";
  html += "document.getElementById('forward').onclick = function() { sendCommand('/forward'); };";
  html += "document.getElementById('backward').onclick = function() { sendCommand('/backward'); };";
  html += "document.getElementById('left').onclick = function() { sendCommand('/left'); };";
  html += "document.getElementById('right').onclick = function() { sendCommand('/right'); };";
  html += "document.getElementById('stop').onclick = function() { sendCommand('/stop'); };";

  // Function to handle AJAX requests
  html += "function sendCommand(command) {";
  html += "  fetch(command)";  // Sends GET request to the server
  html += "    .then(response => response.text())";
  html += "    .then(data => {";
  html += "      console.log(data);";  // Print the response in the console (optional)
  html += "      alert(data);";  // Display the response in an alert box
  html += "    })";
  html += "    .catch(error => console.error('Error:', error));";
  html += "}";

  // Toggle joystick control via the switch
  html += "function toggleJoystickControl() {";
  html += "  var checkbox = document.getElementById('joystickSwitch');";
  html += "  if (checkbox.checked) {";
  html += "    fetch('/switchToJoystick');";
  html += "  } else {";
  html += "    fetch('/switchOffJoystick');";
  html += "  }";
  html += "}";

  html += "</script>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Function to switch to Joystick Control mode
void switchToJoystick() {
  joystickControl = true;
  server.send(200, "text/plain", "Switched to Joystick Control");
}

// Function to switch off Joystick Control mode
void switchOffJoystick() {
  joystickControl = false;
  server.send(200, "text/plain", "Switched to Web Control");
}

// Functions to control the motors
void moveForward() {
  if (!joystickControl) {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    server.send(200, "text/plain", "Moving Forward");
  }
}

void moveBackward() {
  if (!joystickControl) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    server.send(200, "text/plain", "Moving Backward");
  }
}

void turnLeft() {
  if (!joystickControl) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    server.send(200, "text/plain", "Turning Left");
  }
}

void turnRight() {
  if (!joystickControl) {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    server.send(200, "text/plain", "Turning Right");
  }
}

void stopMotors() {
  if (!joystickControl) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    server.send(200, "text/plain", "Motors Stopped");
  }
}
