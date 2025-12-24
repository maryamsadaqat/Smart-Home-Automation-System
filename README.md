# **Smart Home Automation System (C++)**

The Smart Home Automation System is a console-based C++ application that simulates the management of a smart home environment. It allows users to register, log in, create rooms, add smart devices, and control them through a centralized system. The project is built using Object-Oriented Programming principles to model real-world smart home behavior in a structured and maintainable way. The system supports automation through scheduling, monitors energy usage of devices, generates notifications for important events, and preserves system data using file handling. Although currently console-based, the architecture is designed to be extensible for future enhancements such as GUI or IoT integration.

## **Features**

### **User Management**
- Allows users to register and log in securely.
- Each user has independent access to their own rooms and devices.
- Authentication includes basic password validation and exception handling.

### **Room and Device Management**
- Users can create multiple rooms within the smart home.
- Different smart devices can be added to each room.
- Supported device types include Lights, Thermostats, Air Conditioners, Cameras, and Door Locks.

### **Device Control**
- Devices can be turned on or off and controlled individually.
- Each device performs actions specific to its type (e.g., brightness adjustment, temperature control, motion detection).
- Remote control functionality allows device interaction through a unified interface.

### **Scheduling and Automation**
- Users can schedule device actions to run at specific times.
- The scheduler continuously checks the system time and triggers actions automatically.
- Schedules can be added, updated, viewed, or removed.

### **Energy Monitoring**
- Tracks energy consumption of devices based on usage.
- Generates detailed energy usage reports.
- Supports threshold-based warnings when energy usage exceeds limits.

### **Notification System**
- Provides alerts for events such as motion detection, scheduled actions, and energy overuse.
- Notifications are stored and can be reviewed by the user at any time.

### **Data Persistence**
- System data (users, rooms, devices, and device states) is saved to files.
- Data is loaded automatically when the system starts, ensuring continuity across sessions.


## **OOP Concepts Used**
- Encapsulation
- Abstraction
- Inheritance
- Polymorphism
