# Operating System Manager with Web UI Integration

A modern C++ console application integrated with a responsive web-based UI for managing system operations, file management, and control panel functions.

## Features

### 🖥️ Console Application (C++)
- **User Authentication**: Admin and Guest login modes
- **Control Panel**: User account management, date/time settings, network configuration
- **File Management**: Create files, access drives, manage documents and downloads
- **System Information**: RAM monitoring, storage analysis, application launcher
- **Real-time Monitoring**: Background system monitoring with JSON data export

### 🌐 Web UI (HTML/CSS/JavaScript)
- **Responsive Design**: Works on desktop, tablet, and mobile devices
- **Real-time Updates**: Live data synchronization from C++ backend
- **Modern Interface**: Clean, intuitive dashboard with card-based layout
- **Data Visualization**: Live system metrics and status indicators
- **Mobile Optimization**: QR code for easy mobile access

## Architecture

```
┌─────────────────┐    JSON Files    ┌─────────────────┐
│   C++ Backend   │◄────────────────►│   Web UI Frontend│
│                 │                  │                 │
│ • User Mgmt     │                  │ • Dashboard     │
│ • File Ops      │                  │ • Control Panel │
│ • System Info   │                  │ • File Manager  │
│ • App Launcher  │                  │ • Real-time UI  │
└─────────────────┘                  └─────────────────┘
```

## Installation & Setup

### Prerequisites
- **C++ Compiler**: MinGW (Windows), GCC (Linux), or Clang (macOS)
- **Web Browser**: Any modern browser (Chrome, Firefox, Edge, Safari)
- **Windows**: Native support (uses Windows API)

### Quick Start

1. **Download Files**: Ensure all files are in the same directory:
   - `cpp_ui_integration.cpp` (C++ source code)
   - `ui.html` (Web interface)
   - `launch_system.bat` (Windows launcher)
   - `README.md` (This file)

2. **Windows Users**: Double-click `launch_system.bat`
   - Automatically compiles the C++ code
   - Launches the application
   - Opens the web UI in your browser

3. **Manual Compilation**:
   ```bash
   g++ -std=c++17 -o operating_system.exe cpp_ui_integration.cpp
   ./operating_system.exe
   ```

4. **Access UI**: Open `ui.html` in your web browser

## Usage Guide

### Console Application
1. **Login**: Choose Admin (password required) or Guest mode
2. **Navigation**: Use numbered menus to navigate different sections
3. **Operations**: Perform system tasks through the console interface
4. **UI Integration**: Press option 5 to launch the web UI

### Web Interface
1. **Dashboard**: View system overview and quick actions
2. **Control Panel**: Manage user settings and system configuration
3. **File Management**: Create files and access system drives
4. **System Info**: Monitor RAM usage and system details
5. **Real-time Updates**: Data automatically syncs from C++ backend

## Data Communication

The system uses JSON files for communication between C++ and web UI:

### Generated Files
- `ui_data.json` - User information and system status
- `storage_data.json` - Drive storage information
- `ram_data.json` - Memory usage data
- `file_log.json` - File creation logs
- `app_log.json` - Application launch logs

### Real-time Updates
- C++ backend writes JSON data every 5 seconds
- Web UI polls for updates every 3 seconds
- Live status indicators show system health
- Browser connection status monitoring

## File Structure

```
Operating_System_UI/
├── cpp_ui_integration.cpp    # Main C++ application
├── ui.html                   # Web interface
├── launch_system.bat         # Windows launcher script
├── README.md                 # Documentation
├── ui_data.json              # User data (auto-generated)
├── storage_data.json         # Storage info (auto-generated)
├── ram_data.json            # Memory data (auto-generated)
├── file_log.json            # File operations (auto-generated)
└── app_log.json             # App launches (auto-generated)
```

## Technical Details

### C++ Features Used
- **Standard Library**: `<filesystem>`, `<thread>`, `<chrono>`
- **Windows API**: `ShellExecuteA`, `GlobalMemoryStatus`
- **JSON Generation**: Manual JSON string construction
- **Multi-threading**: Background monitoring thread
- **Process Monitoring**: Browser detection via process enumeration

### Web Technologies
- **HTML5**: Semantic markup and structure
- **CSS3**: Modern styling with CSS Grid and Flexbox
- **JavaScript ES6+**: Async/await, fetch API, DOM manipulation
- **JSON**: Data exchange format
- **Responsive Design**: Mobile-first approach

## Troubleshooting

### Compilation Errors
- **MinGW Not Found**: Install MinGW-w64 and add to PATH
- **C++17 Support**: Ensure compiler supports C++17 standard
- **Missing Headers**: Verify all standard library headers are available

### UI Issues
- **JSON Files Not Found**: Ensure C++ application is running
- **Browser Security**: Some features may be restricted in browser sandbox
- **Real-time Updates**: Check file permissions and polling intervals

### Windows-Specific
- **Admin Privileges**: Some operations may require administrator rights
- **Antivirus**: May block process enumeration or file operations
- **Browser Default**: Ensure default browser is properly configured

## Security Notes

- **Local Application**: Runs entirely on local machine
- **No Network**: No external network connections required
- **File System Access**: Limited to user-accessible directories
- **Process Monitoring**: Read-only access to process information

## Customization

### Adding New Features
1. **C++ Side**: Add functions and JSON output
2. **Web Side**: Create UI components and data handlers
3. **Integration**: Update polling and display functions

### Styling Changes
- Modify CSS variables in `ui.html` for theme changes
- Adjust grid layouts for different screen sizes
- Customize color scheme and typography

## License

This project is open source and available under the MIT License.

## Support

For issues, questions, or contributions:
1. Check the troubleshooting section above
2. Review the code comments for implementation details
3. Ensure all prerequisites are properly installed

---

**Note**: This is a demonstration project showing C++ and web technology integration. For production use, consider using frameworks like Electron or native desktop application frameworks.
