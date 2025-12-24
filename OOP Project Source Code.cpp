#include <iostream>
#include <ctime>
#include <map>
#include <vector>
#include <algorithm>
#include <exception>
#include <stdexcept> 
#include <cctype> 
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits>
#include <memory>
using namespace std;

class DeviceException : public exception {
    string errorMessage;
public:
    DeviceException(string msg) : errorMessage(msg) {}
    const char* what() const noexcept override { return errorMessage.c_str(); }
};

class Room;
class Device;
class SmartHome;
class User;  

class Device {
protected:
    string deviceID;
    string deviceName;
    bool status;
    string deviceType;
    string location;
public:
    float powerConsumption;

    Device(string id, string name, string type, string loc)
        : deviceID(id), deviceName(name), deviceType(type), location(loc), status(false) {}

     void turnOn() { status = true; }
     void turnOff() { status = false; }
    virtual bool getStatus() { return status; }

    string getDeviceID() const { return deviceID; }
    string getDeviceName() const { return deviceName; }
    string getLocation() const { return location; }
    string getDeciceType() const { return deviceType; }

    void setLocation(string loc) { location = loc; }

    virtual string getDeviceInfo() {
        return "ID: " + deviceID + "\nName: " + deviceName + "\nType: " + deviceType +
               "\nLocation: " + location + "\nStatus: " + (status ? "On" : "Off");
               
    }
    float getEnergyUsage(float hoursUsed) const {
    return powerConsumption * hoursUsed; 
}
    virtual void performAction() = 0; 
    virtual ~Device(){
	}
};

class Light : public Device {
private:
    float brightnessLevel;

public:
    Light(string id, string name, string loc)
        : Device(id, name, "Light", loc), brightnessLevel(0.0) {}

    void setBrightness(float level) { brightnessLevel = level; }
    float getBrightness() { return brightnessLevel; }

    void performAction() override {
        cout << "Light (" << deviceName << ") dimming to " << brightnessLevel << "% brightness." << endl;
    }
};

class Camera : public Device {
private:
    bool isRecording;
    bool motionDetected;
    string lastMotionTime;

public:
    Camera(string id, string name, string loc)
        : Device(id, name, "Camera", loc), isRecording(false), motionDetected(false), lastMotionTime("") {}

    void startRecording() {
        isRecording = true;
        cout << "Camera (" << deviceName << ") has started recording." << endl;
    }

    void stopRecording() {
        isRecording = false;
        cout << "Camera (" << deviceName << ") has stopped recording." << endl;
    }

    void detectMotion() {
        motionDetected = true;
        time_t now = time(0);
        lastMotionTime = ctime(&now);
        cout << "Motion detected by Camera (" << deviceName << ") at " << lastMotionTime;
    }

    string getLastMotionTime() { return lastMotionTime; }

    void performAction() override {
        cout << "Camera (" << deviceName << ") is monitoring the area." << endl;
        if (motionDetected) {
            cout << "Last motion: " << lastMotionTime;
        } else {
            cout << "No motion detected." << endl;
        }
    }
};

class DoorLock : public Device {
private:
    bool isLocked;

public:
    DoorLock(string id, string name, string loc)
        : Device(id, name, "Door Lock", loc), isLocked(true) {}

    void lockDoor() { isLocked = true; cout << "Door locked.\n"; }
    void unlockDoor() { isLocked = false; cout << "Door unlocked.\n"; }

    bool checkLockStatus() { return isLocked ? "Locked" : "Unlocked"; }

    void performAction() override {
        cout << "DoorLock (" << deviceName << ") is " << (isLocked ? "Locked." : "Unlocked.") << endl;
    }
    ~DoorLock() {
	}
};

class TemperatureControlledDevices : public Device {
protected:
    float currentTemperature;
    float targetTemperature;

public:
    TemperatureControlledDevices(string id, string name, string type, string loc)
        : Device(id, name, type, loc), currentTemperature(25.0f), targetTemperature(25.0f) {}

    void setTemperature(float temp) { targetTemperature = temp; }
    float getCurrentTemperature() const { return currentTemperature; }

    virtual void adjustTemperature() {
        if (currentTemperature < targetTemperature) currentTemperature += 1.0f;
        else if (currentTemperature > targetTemperature) currentTemperature -= 1.0f;
    }

    void performAction() override {}
    virtual ~TemperatureControlledDevices() {}
};

class Thermostat : public TemperatureControlledDevices{ 
private:
    float currentTemperature;
    float targetTemperature;

public:
    Thermostat(string id, string name, string loc)
        : TemperatureControlledDevices (id, name, "Thermostat", loc), currentTemperature(50.0), targetTemperature(50.0) {}

    void performAction() override {
        cout << "Thermostat (" << deviceName << ") is regulating temperature." << endl;
        adjustTemperature();
    }
};

class AirConditioner : public TemperatureControlledDevices {
public:
    AirConditioner(string id, string name, string loc)
        : TemperatureControlledDevices(id, name, "AirConditioner", loc) {}

    void performAction() override {
        adjustTemperature();
        cout << "AC (" << deviceName << ") cooling to " << targetTemperature << "°C. Current: " << currentTemperature << "°C\n";
    }
};

class Notification {
    vector<string> notifications;
public:
    void sendAlert(string msg) { notifications.push_back(msg); }
    void viewAlerts() {
        for (string alert : notifications)
            cout << "Alert: " << alert << endl;
    }
    ~Notification() {}
};

class Room {
private:
    string roomName;
    vector<Device*> devices;

public:
    Room(string name) : roomName(name) {}
    
    string getRoomName() {
        return roomName;
    }
    
    void addDevice(Device* device) {
        devices.push_back(device);
    }

    void removeDevice(string ID) {
        auto it = remove_if(devices.begin(), devices.end(), [&](Device* d) {
            return d->getDeviceID() == ID;
        });
        if (it != devices.end()) {
            devices.erase(it, devices.end());
        }
    }

    Device* getDevicesByName(string name) {
        for (Device* d : devices) {
            if (d->getDeviceName() == name) {
                return d;
            }
        }
        return nullptr;
    }
    vector<Device*> getDevices() {
    return devices;
}
    void displayDevices() const {
        cout << "Devices in room '" << roomName << "':\n";
        for (Device* d : devices) {
            cout << d->getDeviceInfo() << "\n\n";
        }
    }

    ~Room() {
        for (Device* d : devices) delete d;
    }
};

    class User {
        string UserID, UserName, Password;
        map<string, Room*> rooms;
    public:
    User(string uname, string pwd) : UserName(uname), Password(pwd) {}
    void registerAccount() { cout << "Account registered for " << UserName << endl; }
    bool authenticate(const string& inputPassword) {        
    if (inputPassword.length() < 6)
        throw DeviceException("Error: Password must have 6 characters or more");

    bool hasDigit = false;
    for (char c : inputPassword) {
        if (isdigit(c)) {
            hasDigit = true;
            break;
        }
    }

    if (!hasDigit)
        throw DeviceException("Error: Password must contain 1 digit");

    if (inputPassword != Password)
        throw DeviceException("Error: Incorrect password");

    return true;
}

    string getUserID (){
	return UserID;
}

    string getUsername(){
	return UserName;
}
    string getPassword(){
	return Password;
}


    bool login(string name, string pass) const {
        return UserName == name && Password == pass;
    }
    bool addRoom(Room* room) {
    if (rooms.count(room->getRoomName()) == 0) {
        rooms[room->getRoomName()] = room;
        return true;
    }
    return false;
}

    bool removeRoom(string roomName) {
        auto it = rooms.find(roomName);
        if (it != rooms.end()) {
            delete it->second;
            rooms.erase(it);
            return true;
        }
        return false;
    }
    Room* getRoom(string roomName) {
        return hasRoom(roomName) ? rooms[roomName] : nullptr;
    }
    const map<string, Room*> getAllRooms() const { return rooms; }

    void viewAllRooms() const {
        cout << "Rooms of user " << UserName << ":\n";
        for (auto& pair : rooms) {
            cout << "- " << pair.first << endl;
            pair.second->displayDevices();
        }
    }
    void viewDevicesInRoom(string roomName) const {
        auto it = rooms.find(roomName);
        if (it != rooms.end()) {
            it->second->displayDevices();
        } else {
            cout << "Room not found.\n";
        }
    }
    bool hasRoom(string name) { return rooms.count(name) > 0; }
    bool addDeviceToRoom(string roomName, Device* device) {
        if (rooms.count(roomName)) { rooms[roomName]->addDevice(device); return true; }
        return false;
    }
    ~User() {
        for (auto& r : rooms) delete r.second;
    }
};


class SmartHome {
    map<string, User*> Users;
public:
    SmartHome() {}  
    
    void addUser(string ID, User* user) { Users[ID] = user; }
    
    void removeUser(string ID) { Users.erase(ID); }

    User* getUser(string name) { 
        auto it = Users.find(name);
        return (it != Users.end()) ? it->second : nullptr;
    }

    void viewAllRoomsAndDevices(const string& userName) {
        User* user = getUser(userName);
        if (user) user->viewAllRooms();
        else cout << "User not found.\n";
    }
    
    bool loginUser(string name, string pwd) {
    if (Users.count(name)) {
        try {
            return Users[name]->authenticate(pwd);
        } catch (const DeviceException& e) {
            cout << e.what() << endl;
            return false;
        }
    }
    return false;
}
    
    const map<string, User*> getAllUsers() const {
        return Users;
    }
    
    void notifyUser(string msg) { Notification n; n.sendAlert(msg); }
    
    void viewSystemStatus() {
        cout << "Smart Home Users:\n";
        for (const auto& pair : Users) {
            cout << "- " << pair.first << endl;
        }
    }
    
    ~SmartHome() {
        for (auto& pair : Users) {
            delete pair.second;
        }
    }
};

class ConsoleUI {
    SmartHome* smartHome;
    User* currentUser;
public:
    ConsoleUI(SmartHome* sh) : smartHome(sh), currentUser(nullptr) {}
    
    User* getCurrentUser() const { return currentUser; }
    void displayMenu() {
        cout << "\n===== Smart Home Menu =====\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Add Room\n";
        cout << "4. Add Device\n";
        cout << "5. View Devices in a Room\n";
        cout << "6. Dashboard\n";
        cout << "7. Device Control (Remote)\n";
        cout << "8. Scheduling\n";
        cout << "9. Energy Report\n";
        cout << "10. Check Schedules\n";
        cout << "0. Exit\n";
        cout << "Choose an option: ";
    }

    void promptRegistration() {
        string username, password;
        cout << "Enter username: "; cin >> username;
        cout << "Enter password: "; cin >> password;
        User* newUser = new User(username, password);
        newUser->registerAccount();
        smartHome->addUser(username, newUser);
    }
    void promptLogin() {
        string username, password;
        cout << "Enter username: "; cin >> username;
        cout << "Enter password: "; cin >> password;
        if (smartHome->loginUser(username, password)) {
            currentUser = smartHome->getUser(username);
            cout << "Login successful!\n";
        } else {
            cout << "Login failed!\n";
        }
    }

    void userDashboard() {
        if (!currentUser) {
           cout << "Please login to access dashboard.\n";
            return;
        }
        cout << "\n===== User Dashboard =====\n";
        currentUser->viewAllRooms();
    }

    void handleUserCommands() {
        int choice;
        do {
            displayMenu();
           cin >> choice;
            switch (choice) {
                case 1: promptRegistration(); break;
                case 2: promptLogin(); break;
                case 3: {
                    if (currentUser) {
                        string roomName;
                        cout << "Enter room name: "; cin >> roomName;
                        Room* room = new Room(roomName);
                        currentUser->addRoom(room);
                        cout << "Room added.\n";
                    } else cout << "Please login first.\n";
                    break;
                }
                case 4: {
    if (currentUser) {
        string roomName, deviceType, id, name;
        cout << "Enter room name to add device: ";
        cin >> roomName;
        cout << "Enter device ID: ";
        cin >> id;
        cout << "Enter device name: ";
        cin >> name;
        cout << "Enter device type (Light/Thermostat/Camera/DoorLock/AC): ";
        cin >> deviceType;

        Device* device = nullptr;
        if (deviceType == "Light")
            device = new Light(id, name, roomName);
        else if (deviceType == "Thermostat")
            device = new Thermostat(id, name, roomName);
        else if (deviceType == "Camera")
            device = new Camera(id, name, roomName);
        else if (deviceType == "DoorLock")
            device = new DoorLock(id, name, roomName);
        else if (deviceType == "AC")
            device = new AirConditioner(id, name, roomName);
        else {
            cout << "Invalid device type.\n";
            break;
        }

        if (!currentUser->addDeviceToRoom(roomName, device)) {
            cout << "Room not found. Device not added.\n";
            delete device;
        } else {
            cout << "Device added successfully.\n";
        }
    } else {
        cout << "Please login first.\n";
    }
    break;
}

                case 5: {
                    if (currentUser) {
                        string roomName;
                        cout << "Enter room name to view devices: "; cin >> roomName;
                        currentUser->viewDevicesInRoom(roomName);
                    } else cout << "Please login first.\n";
                    break;
                }
                case 6: userDashboard(); break;
                case 7: cout << "Exiting...\n"; break;
                default: cout << "Invalid choice.\n";
            }
        } while (choice != 7);
    }

    ~ConsoleUI() {
        cout << "ConsoleUI shutting down...\n";
    }
};


struct Time {
    int hour;
    int minute;

    Time() : hour(0), minute(0) {}
    Time(int h, int m) : hour(h), minute(m) {}

    bool operator==(const Time& other) const {
        return hour == other.hour && minute == other.minute;
    }

    string toString() const {
        stringstream ss;
        ss << setw(2) << setfill('0') << hour << ":" << setw(2) << setfill('0') << minute;
        return ss.str();
    }
};

class Scheduler {
private:
    map<Device*, Time> schedules;

    Time getCurrentTime() const {
        time_t now = time(0);
        tm* localTime = localtime(&now);
        return Time(localTime->tm_hour, localTime->tm_min);
    }

public:
    void addSchedule(Device* device, Time time) {
        schedules[device] = time;
        cout << "Scheduled device at " << time.toString() << endl;
    }

    void removeSchedule(Device* device) {
        if (schedules.erase(device)) {
            cout << "Schedule removed for device.\n";
        } else {
            cout << "No schedule found for device.\n";
        }
    }

    void checkAndRunSchedules() {
        Time currentTime = getCurrentTime();
        for (map<Device*, Time>::iterator it = schedules.begin(); it != schedules.end(); ++it) {
            if (it->second == currentTime) {
                cout << "Running scheduled action at " << currentTime.toString() << endl;
                it->first->performAction();
            }
        }
    }

    void updateSchedule(Device* device, Time newTime) {
        if (schedules.find(device) != schedules.end()) {
            schedules[device] = newTime;
            cout << "Schedule updated to " << newTime.toString() << endl;
        } else {
            cout << "Device not found in schedule.\n";
        }
    }

    void listSchedules() {
        cout << "Scheduled Devices:\n";
        for (map<Device*, Time>::iterator it = schedules.begin(); it != schedules.end(); ++it) {
            cout << "- Device at " << it->second.toString() << endl;
        }
    }

    Time getSchedule(Device* device) {
        if (schedules.find(device) != schedules.end()) {
            return schedules[device];
        }
        return Time(-1, -1); // Indicates not found
    }

    void clearAllSchedules() {
        schedules.clear();
        cout << "All schedules cleared.\n";
    }

    ~Scheduler() {
    }
};

class EnergyMonitor {
private:
    map<string, float> energyUsage;
    float threshold;

public:
    EnergyMonitor() : threshold(30.0f) {}

    void recordUsage(const string& deviceID, float amount) {
        energyUsage[deviceID] += amount;
        cout << "Recorded " << amount << " units for device: " << deviceID << endl;
    }

    float getUsage(const string& deviceID) const {
        auto it = energyUsage.find(deviceID);
        if (it != energyUsage.end()) {
            return it->second;
        }
        return 0.0f;
    }

    float getTotalUsage() const {
        float total = 0.0f;
        for (auto& entry : energyUsage) {
            total += entry.second;
        }
        return total;
    }

    void setThreshold(float value) {
        threshold = value;
        cout << "Energy threshold set to " << threshold << " units.\n";
    }

    float getThreshold() const {
        return threshold;
    }

    void checkThreshold() const {
        float total = getTotalUsage();
        if (total > threshold) {
            cout << "️ Warning: Energy usage exceeded threshold! ("
                 << total << " > " << threshold << ")\n";
        } else {
            cout << "✅ Energy usage within threshold. (" << total << " <= " << threshold << ")\n";
        }
    }

    void displayUsageReport() const {
        cout << "\n--- Energy Usage Report ---\n";
        for (auto& entry : energyUsage) {
            cout << "Device ID: " << entry.first
                 << " | Usage: " << fixed << setprecision(2)
                 << entry.second << " units\n";
        }
        cout << "Total Usage: " << fixed << setprecision(2)
             << getTotalUsage() << " units\n";
        cout << "Threshold: " << fixed << setprecision(2)
             << threshold << " units\n";
        cout << "----------------------------\n";
    }
    ~ EnergyMonitor (){
	}
};

class RemoteControl {
private:
    User* user;

public:
    RemoteControl(User* u) : user(u) {}

    bool turnDeviceOn(string roomName, string deviceName) {
        Room* room = user->getRoom(roomName);
        if (room) {
            Device* device = room->getDevicesByName(deviceName);
            if (device) {
                device->turnOn();
                cout << "Turned ON device: " << deviceName << " in " << roomName << endl;
                return true;
            }
        }
        cout << "Failed to turn ON device. Room or device not found." << endl;
        return false;
    }

    bool turnDeviceOff(string roomName, string deviceName) {
        Room* room = user->getRoom(roomName);
        if (room) {
            Device* device = room->getDevicesByName(deviceName);
            if (device) {
                device->turnOff();
                cout << "Turned OFF device: " << deviceName << " in " << roomName << endl;
                return true;
            }
        }
        cout << "Failed to turn OFF device. Room or device not found." << endl;
        return false;
    }

    bool performDeviceAction(string roomName, string deviceName) {
        Room* room = user->getRoom(roomName);
        if (room) {
            Device* device = room->getDevicesByName(deviceName);
            if (device) {
                device->performAction();
                return true;
            }
        }
        cout << "Failed to perform action. Room or device not found." << endl;
        return false;
    }

    void listAllDevices() {
        map<string, Room*> rooms = user->getAllRooms();
        for (auto& pair : rooms) {
            cout << "Room: " << pair.first << endl;
            pair.second->displayDevices();
        }
    }

    ~RemoteControl() {
        cout << "RemoteControl disconnected.\n";
    }
};

class DataStorage {
private:
    string filename;

public:
    DataStorage(const string& fname) : filename(fname) {}

    void saveSystem(SmartHome* smartHome) {
        ofstream out(filename, ios::trunc);
        if (!out.is_open()) {
            throw DeviceException("Cannot open file for writing: " + filename);
        }

        for (const auto& [username, user] : smartHome->getAllUsers()) {
            out << "USER " << username << " " << user->getPassword() << "\n";

            for (const auto& [roomName, room] : user->getAllRooms()) {
                out << "ROOM " << roomName << "\n";
                
                for (Device* device : room->getDevices()) {
                    out << "DEVICE " << device->getDeciceType() << " "
                        << device->getDeviceID() << " "
                        << device->getDeviceName() << " "
                        << device->getLocation() << " "
                        << device->getStatus() << " "
                        << device->powerConsumption << " ";
                    
                    if (auto light = dynamic_cast<Light*>(device)) {
                        out << light->getBrightness();
                    } 
                    else if (auto thermo = dynamic_cast<Thermostat*>(device)) {
                        out << thermo->getCurrentTemperature();
                    }
                    else if (auto camera = dynamic_cast<Camera*>(device)) {
                        out << (camera->getLastMotionTime().empty() ? "NoMotion" : camera->getLastMotionTime());
                    }
                    else if (auto doorLock = dynamic_cast<DoorLock*>(device)) {
                        out << doorLock->checkLockStatus();
                    }
                    else if (auto ac = dynamic_cast<AirConditioner*>(device)) {
                        out << ac->getCurrentTemperature();
                    }
                    out << "\n";
                }
            }
        }
        out.close();
    }

    void loadSystem(SmartHome* smartHome) {
        ifstream in(filename);
        if (!in.is_open()) {
            return; 

        User* currentUser = nullptr;
        Room* currentRoom = nullptr;
        string line;

        while (getline(in, line)) {
            stringstream ss(line);
            string type;
            ss >> type;

            if (type == "USER") {
                string username, password;
                ss >> username >> password;
                currentUser = new User(username, password);
                smartHome->addUser(username, currentUser);
            }
            else if (type == "ROOM") {
                string roomName;
                ss >> roomName;
                if (currentUser) {
                    currentRoom = new Room(roomName);
                    currentUser->addRoom(currentRoom);
                }
            }
            else if (type == "DEVICE") {
                string devType, id, name, loc;
                bool status;
                float power;
                ss >> devType >> id >> name >> loc >> status >> power;

                Device* device = nullptr;
                if (devType == "Light") {
                    float brightness;
                    ss >> brightness;
                    device = new Light(id, name, loc);
                    dynamic_cast<Light*>(device)->setBrightness(brightness);
                }
                else if (devType == "Thermostat") {
                    float temp;
                    ss >> temp;
                    device = new Thermostat(id, name, loc);
                    dynamic_cast<Thermostat*>(device)->setTemperature(temp);
                }

                if (device) {
                    device->powerConsumption = power;
                    if (status) device->turnOn();
                    else device->turnOff();

                    if (currentRoom) {
                        currentRoom->addDevice(device);
                    }
                }
            }
        }
        in.close();
    }
}
        
    void saveUser(User* user) {
        ofstream out(filename, ios::app);  
        if (!out.is_open()) {
            cerr << "Cannot open file to save user: " << filename << endl;
            return;
        }

        out << "USER " << user->getUsername() << " " << user->getPassword() << "\n";

        auto& rooms = user->getAllRooms(); 
        for (auto it = rooms.begin(); it != rooms.end(); ++it) {
            const string& roomName = it->first;  
            Room* room = it->second;
            out << "ROOM " << roomName << "\n";

            for (Device* device : room->getDevices()) {
                saveDevice(device);
            }
        }

        out.close();
    }

    vector<User*> loadUsers() {
        ifstream in(filename);
        vector<User*> users;

        if (!in.is_open() || in.peek() == ifstream::traits_type::eof()) {
            return users; 
        }

        string type;
        User* currentUser = nullptr;
        Room* currentRoom = nullptr;

        while (in >> type) {
            if (type == "USER") {
                string username, password;
                in >> username >> password;
                currentUser = new User(username, password);
                users.push_back(currentUser);
            } else if (type == "ROOM") {
                string roomName;
                in >> roomName;
                currentRoom = new Room(roomName);
                if (currentUser) {
                    currentUser->addRoom(currentRoom);
                }
            } else if (type == "DEVICE") {
                string devType, id, name, loc;
                int status;
                in >> devType >> id >> name >> loc >> status;

                Device* dev = nullptr;
                if (devType == "Light") dev = new Light(id, name, loc);
                else if (devType == "Thermostat") dev = new Thermostat(id, name, loc);
                else if (devType == "Camera") dev = new Camera(id, name, loc);
                else if (devType == "DoorLock") dev = new DoorLock(id, name, loc);
                else if (devType == "AC") dev = new AirConditioner(id, name, loc);

                if (dev) {
                    if (status) dev->turnOn();
                    else dev->turnOff();

                    if (currentRoom) {
                        currentRoom->addDevice(dev);
                    }
                }
            }
        }

        in.close();
        return users;
    }

    void saveDevice(Device* device) {
        ofstream out(filename, ios::app);  
        if (!out.is_open()) {
            cerr << "Cannot open file to save device: " << filename << endl;
            return;
        }

        out << "DEVICE " << device->getDeciceType() << " "
            << device->getDeviceID() << " "
            << device->getDeviceName() << " "
            << device->getStatus() << "\n";

        out.close();
    }

    vector<Device*> loadDevices() {
        ifstream in(filename);
        vector<Device*> devices;
        if (!in.is_open()) {
            cerr << "Cannot open file to load devices: " << filename << endl;
            return devices;
        }

        string type;
        while (in >> type) {
            if (type == "DEVICE") {
                string devType, id, name, loc;
                int status;
                in >> devType >> id >> name >> loc >> status;

                Device* dev = nullptr;
                if (devType == "Light") dev = new Light(id, name, loc);
                else if (devType == "Thermostat") dev = new Thermostat(id, name, loc);
                else if (devType == "Camera") dev = new Camera(id, name, loc);
                else if (devType == "DoorLock") dev = new DoorLock(id, name, loc);
                else if (devType == "AC") dev = new AirConditioner(id, name, loc);

                if (dev) {
                    if (status) dev->turnOn();
                    else dev->turnOff();

                    devices.push_back(dev);
                }
            }
        }

        in.close();
        return devices;
    }

    void clearStorage() {
        ofstream out(filename, ios::trunc);
        out.close();
    }


};

int main() {
    SmartHome smartHome;
    DataStorage storage("data.txt");
    EnergyMonitor energyMonitor;
    Scheduler scheduler;
    Notification notifications;
    
    // Load existing data at startup
    try {
        vector<User*> loadedUsers = storage.loadUsers();
        for (User* user : loadedUsers) {
            smartHome.addUser(user->getUsername(), user);
        }
    } catch (const exception& e) {
        cout << "Error loading data: " << e.what() << "\nStarting with empty system.\n";
    }

    ConsoleUI ui(&smartHome);
    unique_ptr<RemoteControl> remote;
    User* currentUser = nullptr;

    while (true) {
        try {
            ui.displayMenu();
            int choice;
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: { // Registration
                    string username, password;
                    cout << "Enter username: ";
                    getline(cin, username);
                    
                    // Check if username already exists
                    if (smartHome.getUser(username) != nullptr) {
                        cout << "Username already exists! Please choose a different username.\n";
                        break;
                    }

                    cout << "Enter password (min 6 chars with at least 1 digit): ";
                    getline(cin, password);

                    // Password validation
                    if (password.length() < 6) {
                        cout << "Password must be at least 6 characters long.\n";
                        break;
                    }
                    if (none_of(password.begin(), password.end(), ::isdigit)) {
                        cout << "Password must contain at least 1 digit.\n";
                        break;
                    }

                    User* newUser = new User(username, password);
                    smartHome.addUser(username, newUser);
                    storage.saveUser(newUser);  // Save the new user immediately
                    cout << "Registration successful!\n";
                    break;
                }
                case 2: { // Login
                    string username, password;
                    cout << "Enter username: ";
                    getline(cin, username);
                    cout << "Enter password: ";
                    getline(cin, password);

                    if (smartHome.loginUser(username, password)) {
                        currentUser = smartHome.getUser(username);
                        remote = make_unique<RemoteControl>(currentUser);
                        cout << "Login successful! Welcome " << username << "!\n";
                        
                        // Load user-specific data
                        try {
                            storage.loadSystem(&smartHome);
                        } catch (const exception& e) {
                            cout << "Warning: Couldn't load user data: " << e.what() << "\n";
                        }
                    } else {
                        cout << "Invalid credentials!\n";
                    }
                    break;
                }
                case 3: { // Add Room
                    if (!currentUser) {
                        cout << "Please login first!\n";
                        break;
                    }
                    string roomName;
                    cout << "Enter room name: ";
                    getline(cin, roomName);
                    
                    if (currentUser->hasRoom(roomName)) {
                        cout << "Room already exists!\n";
                    } else {
                        currentUser->addRoom(new Room(roomName));
                        storage.saveSystem(&smartHome);  // Save after adding room
                        cout << "Room added successfully!\n";
                    }
                    break;
                }
                case 4: { // Add Device
                    if (!currentUser) {
                        cout << "Please login first!\n";
                        break;
                    }

                    string roomName, deviceType, id, name;
                    cout << "Enter room name: ";
                    getline(cin, roomName);
                    
                    if (!currentUser->hasRoom(roomName)) {
                        cout << "Room not found!\n";
                        break;
                    }

                    cout << "Enter device ID: ";
                    getline(cin, id);
                    cout << "Enter device name: ";
                    getline(cin, name);
                    cout << "Enter device type (Light/Thermostat/Camera/DoorLock/AC): ";
                    getline(cin, deviceType);

                    Device* device = nullptr;
                    if (deviceType == "Light") {
                        device = new Light(id, name, roomName);
                        device->powerConsumption = 0.1f;
                        cout << "Enter initial brightness (0-100): ";
                        float brightness;
                        cin >> brightness;
                        cin.ignore();
                        dynamic_cast<Light*>(device)->setBrightness(brightness);
                    } 
                    else if (deviceType == "Thermostat") {
                        device = new Thermostat(id, name, roomName);
                        device->powerConsumption = 0.5f;
                        cout << "Enter initial temperature: ";
                        float temp;
                        cin >> temp;
                        cin.ignore();
                        dynamic_cast<Thermostat*>(device)->setTemperature(temp);
                    } 
					else if (deviceType == "Camera") {
                        device = new Camera(id, name, roomName);
                        device->powerConsumption = 0.05f;
                    } else if (deviceType == "DoorLock") {
                        device = new DoorLock(id, name, roomName);
                        device->powerConsumption = 0.02f;
                    } else if (deviceType == "AC") {
                        device = new AirConditioner(id, name, roomName);
                        device->powerConsumption = 1.5f;
                        cout << "Enter initial temperature: ";
                        float temp;
                        cin >> temp;
                        cin.ignore();
                        dynamic_cast<AirConditioner*>(device)->setTemperature(temp);
                    } else {
                        cout << "Invalid device type!\n";
                        break;
                    }

                    currentUser->addDeviceToRoom(roomName, device);
                    storage.saveSystem(&smartHome);
                    cout << "Device added successfully!\n";
                    break;
                }
                case 5: { // View Devices
                    if (!currentUser) {
                        cout << "Please login first!\n";
                        break;
                    }
                    string roomName;
                    cout << "Enter room name: ";
                    getline(cin, roomName);
                    currentUser->viewDevicesInRoom(roomName);
                    break;
                }
                case 6: { // Dashboard
                    if (currentUser) {
                        currentUser->viewAllRooms();
                    } else {
                        cout << "Please login first!\n";
                    }
                    break;
                }
                case 7: { // Remote Control
                    if (!currentUser || !remote) {
                        cout << "Please login first!\n";
                        break;
                    }

                    string roomName, deviceName;
                    cout << "Enter room name: ";
                    getline(cin, roomName);
                    cout << "Enter device name: ";
                    getline(cin, deviceName);

                    Device* device = currentUser->getRoom(roomName)->getDevicesByName(deviceName);
                    if (!device) {
                        cout << "Device not found!\n";
                        break;
                    }

                    cout << "Choose operation:\n";
                    if (dynamic_cast<Light*>(device)) {
                        cout << "1. Turn On\n2. Turn Off\n3. Set Brightness\n";
                    } else if (dynamic_cast<Thermostat*>(device) || dynamic_cast<AirConditioner*>(device)) {
                        cout << "1. Turn On\n2. Turn Off\n3. Set Temperature\n";
                    } else if (dynamic_cast<Camera*>(device)) {
                        cout << "1. Start Recording\n2. Stop Recording\n3. Detect Motion\n";
                    } else if (dynamic_cast<DoorLock*>(device)) {
                        cout << "1. Lock\n2. Unlock\n3. Check Status\n";
                    } else {
                        cout << "1. Turn On\n2. Turn Off\n3. Perform Action\n";
                    }

                    int op;
                    cin >> op;
                    cin.ignore();

                    switch (op) {
                        case 1:
                            if (dynamic_cast<Camera*>(device)) {
                                dynamic_cast<Camera*>(device)->startRecording();
                                cout << "Recording started for " << deviceName << endl;
                            } else {
                                device->turnOn();
                                cout << deviceName << " turned on\n";
                            }
                            break;
                        case 2:
                            if (dynamic_cast<Camera*>(device)) {
                                dynamic_cast<Camera*>(device)->stopRecording();
                                cout << "Recording stopped for " << deviceName << endl;
                            } else {
                                device->turnOff();
                                cout << deviceName << " turned off\n";
                            }
                            break;
                        case 3:
                            if (dynamic_cast<Light*>(device)) {
                                cout << "Enter brightness (0-100): ";
                                float brightness;
                                cin >> brightness;
                                cin.ignore();
                                dynamic_cast<Light*>(device)->setBrightness(brightness);
                                cout << "Brightness set to " << brightness << "%\n";
                            } else if (dynamic_cast<Thermostat*>(device) || dynamic_cast<AirConditioner*>(device)) {
                                cout << "Enter temperature: ";
                                float temp;
                                cin >> temp;
                                cin.ignore();
                                dynamic_cast<TemperatureControlledDevices*>(device)->setTemperature(temp);
                                cout << "Temperature set to " << temp << "°\n";
                            } else if (dynamic_cast<Camera*>(device)) {
                                dynamic_cast<Camera*>(device)->detectMotion();
                                notifications.sendAlert("Motion detected by " + deviceName);
                                cout << "Motion detection activated\n";
                            } else if (dynamic_cast<DoorLock*>(device)) {
                                cout << "Door is " << (dynamic_cast<DoorLock*>(device)->checkLockStatus() ? "locked" : "unlocked") << endl;
                            } else {
                                device->performAction();
                            }
                            break;
                        default:
                            cout << "Invalid operation!\n";
                    }

                    // Record energy usage only if device is on
                    if (device->getStatus()) {
    float usage = device->getEnergyUsage(0.1f); // 6 minutes of usage
    energyMonitor.recordUsage(device->getDeviceID(), usage);
    cout << "Energy used: " << fixed << setprecision(2) << usage 
         << " kWh (Power: " << device->powerConsumption << " kW)\n";} // Add power display
                    storage.saveSystem(&smartHome);
                    break;
                
            }
                case 8: { // Scheduling
                    if (!currentUser) {
                        cout << "Please login first!\n";
                        break;
                    }

                    string roomName, deviceName;
                    int hour, minute;
                    cout << "Enter room name: ";
                    getline(cin, roomName);
                    cout << "Enter device name: ";
                    getline(cin, deviceName);
                    cout << "Enter time (HH MM): ";
                    cin >> hour >> minute;
                    cin.ignore();

                    Device* device = currentUser->getRoom(roomName)->getDevicesByName(deviceName);
                    if (device) {
                        scheduler.addSchedule(device, Time(hour, minute));
                        cout << "Device scheduled successfully at " 
                             << setw(2) << setfill('0') << hour << ":" 
                             << setw(2) << setfill('0') << minute << "!\n";
                        notifications.sendAlert("Device " + deviceName + " scheduled");
                        storage.saveSystem(&smartHome);
                    } else {
                        cout << "Device not found!\n";
                    }
                    break;
                }
                case 9: { // Energy Report
                    energyMonitor.displayUsageReport();
                    break;
                }
                case 10: { // Notifications
                    notifications.viewAlerts();
                    break;
                }
                case 0: { // Exit
                    storage.saveSystem(&smartHome);
                    cout << "Goodbye!\n";
                    return 0;
                }
                default:
                    cout << "Invalid choice!\n";
            }

            // Check and run scheduled tasks
            scheduler.checkAndRunSchedules();
            
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return 0;
}


