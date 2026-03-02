#include <iostream>
#include <string>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <fstream>
#include <windows.h>
#include <tlhelp32.h>
#include <sstream>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace std;

// Global variables for UI communication
string current_username = "User";
string current_password = "1234";
bool ui_launched = false;

// Function to get current timestamp
string getCurrentTimestamp()
{
    auto now = chrono::system_clock::now();
    time_t time_t_now = chrono::system_clock::to_time_t(now);
    return ctime(&time_t_now);
}

// Function to write data to JSON file for UI
void writeUIData(const string &data)
{
    ofstream file("ui_data.json");
    if (file.is_open())
    {
        file << data;
        file.close();
    }
}

// Function to read data from JSON file
string readUIData()
{
    ifstream file("ui_data.json");
    string data;
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            data += line;
        }
        file.close();
    }
    return data;
}

// Function to launch UI in browser
void launchUI()
{
    if (ui_launched)
    {
        cout << "UI is already running!\n";
        return;
    }

    string html_path = "ui.html";
    filesystem::path absolute_path = filesystem::current_path() / html_path;
    string url = "file:///" + absolute_path.string();

    // Replace backslashes with forward slashes for URL
    replace(url.begin(), url.end(), '\\', '/');

    cout << "Launching UI in browser...\n";
    cout << "URL: " << url << "\n\n";

    // Launch in default browser
    ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
    ui_launched = true;

    // Update UI data
    string ui_data = R"({
        "username": ")" +
                     current_username + R"(",
        "timestamp": ")" +
                     getCurrentTimestamp() + R"(",
        "status": "active"
    })";
    writeUIData(ui_data);
}

// Function to check if browser process is running
bool isBrowserRunning()
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);
        return false;
    }

    do
    {
        string processName = pe32.szExeFile;
        // Check for common browser processes
        if (processName.find("chrome.exe") != string::npos ||
            processName.find("firefox.exe") != string::npos ||
            processName.find("msedge.exe") != string::npos ||
            processName.find("opera.exe") != string::npos)
        {
            CloseHandle(hProcessSnap);
            return true;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return false;
}

// Enhanced storage function with JSON output
void storage(char target)
{
    cout << "Enter Drive type [C:,D:,E:] :";
    cin >> target;
    cout << "\n";
    string drive_path = "";
    drive_path += target;
    drive_path += ":\\";
    filesystem::path target_drive = drive_path;

    try
    {
        filesystem::space_info drive_space = filesystem::space(target_drive);
        constexpr double BYTES_IN_GB = 1024.0 * 1024.0 * 1024.0;
        cout << fixed << setprecision(2);
        cout << "-----Storage Info for " << target << "-----\n";
        cout << "Total Capacity : " << drive_space.capacity / BYTES_IN_GB << "GB\n";
        cout << "Free Space : " << drive_space.free / BYTES_IN_GB << "GB\n";
        cout << "Available Space : " << drive_space.available / BYTES_IN_GB << "GB\n";

        // Write storage data to JSON for UI
        string storage_data = R"({
            "drive": ")" + string(1, target) +
                              R"(",
            "total_gb": )" + to_string((int)(drive_space.capacity / BYTES_IN_GB)) +
                              R"(,
            "free_gb": )" + to_string((int)(drive_space.free / BYTES_IN_GB)) +
                              R"(,
            "available_gb": )" +
                              to_string((int)(drive_space.available / BYTES_IN_GB)) + R"(
        })";

        // Write to storage_data.json
        ofstream storage_file("storage_data.json");
        if (storage_file.is_open())
        {
            storage_file << storage_data;
            storage_file.close();
        }
    }
    catch (const filesystem::filesystem_error &e)
    {
        cerr << "Filesystem Error :" << e.what() << "\n";
    }
}

// Enhanced file creation with JSON logging
void makefile(string filename, string enter_data)
{
    cout << "New Folder[.txt,.html]: ";
    cin >> filename;
    ofstream myfile(filename);
    cout << "You can save anything ....\n>>";
    cin >> enter_data;
    cin.ignore();
    getline(cin, enter_data);
    myfile << enter_data;
    cout << "Saves Files " << filename << "\n\n";
    myfile.close();

    // Log file creation to JSON
    string file_log = R"({
        "filename": ")" +
                      filename + R"(",
        "content_length": )" +
                      to_string(enter_data.length()) + R"(,
        "timestamp": ")" +
                      getCurrentTimestamp() + R"(",
        "status": "created"
    })";

    // Append to file_log.json
    ofstream log_file("file_log.json", ios::app);
    if (log_file.is_open())
    {
        log_file << file_log << "\n";
        log_file.close();
    }
}

// Enhanced application launcher with process monitoring
void onAppButton()
{
    int desktop;
    cout << "1.Python\n2.Chrome\n3.Youtube\n4.Email\n5.CMD\n>>";
    cin >> desktop;

    HINSTANCE result;
    string app_name;

    switch (desktop)
    {
    case 1:
        cout << "Opening Python...\n";
        result = ShellExecuteA(NULL, "open", "python.exe", NULL, NULL, SW_SHOW);
        app_name = "Python";
        break;
    case 2:
        cout << "Opening Chrome...\n";
        result = ShellExecuteA(NULL, "open", "chrome.exe", NULL, NULL, SW_SHOW);
        app_name = "Chrome";
        break;
    case 3:
        cout << "Opening Youtube...\n";
        result = ShellExecuteA(NULL, "open", "https://www.youtube.com", NULL, NULL, SW_SHOWNORMAL);
        app_name = "YouTube";
        break;
    case 4:
        cout << "Opening Email...\n";
        result = ShellExecuteA(NULL, "open", "https://www.gmail.com", NULL, NULL, SW_SHOWNORMAL);
        app_name = "Gmail";
        break;
    case 5:
        cout << "Opening Terminal...\n";
        result = ShellExecuteA(NULL, "open", "cmd.exe", NULL, NULL, SW_SHOW);
        app_name = "Command Prompt";
        break;
    default:
        cout << "Invalid Option\n";
        return;
    }

    if ((intptr_t)result <= 32)
    {
        cout << "Error: Could not open " << app_name << ". (Code: " << (intptr_t)result << ")\n";
    }
    else
    {
        cout << "Launched " << app_name << " successfully!\n";

        // Log app launch to JSON
        string app_log = R"({
            "app_name": ")" +
                         app_name + R"(",
            "timestamp": ")" +
                         getCurrentTimestamp() + R"(",
            "status": "launched"
        })";

        ofstream log_file("app_log.json", ios::app);
        if (log_file.is_open())
        {
            log_file << app_log << "\n";
            log_file.close();
        }
    }
}

// Enhanced RAM monitoring with JSON output
void ram()
{
    MEMORYSTATUS statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatus(&statex);
    cout << "Total Ram:" << statex.dwTotalPhys / (1024 * 1024 * 1024) << "GB" << endl;

    // Write RAM data to JSON for UI
    string ram_data = R"({
        "total_gb": )" +
                      to_string((int)(statex.dwTotalPhys / (1024 * 1024 * 1024))) + R"(,
        "free_mb": )" +
                      to_string((int)(statex.dwAvailPhys / (1024 * 1024))) + R"(,
        "timestamp": ")" +
                      getCurrentTimestamp() + R"("
    })";

    ofstream ram_file("ram_data.json");
    if (ram_file.is_open())
    {
        ram_file << ram_data;
        ram_file.close();
    }
}

// Function to monitor system and update UI data periodically
void monitorSystem()
{
    while (ui_launched)
    {
        // Update user data
        string ui_data = R"({
            "username": ")" +
                         current_username + R"(",
            "timestamp": ")" +
                         getCurrentTimestamp() + R"(",
            "status": "active",
            "browser_running": )" +
                         (isBrowserRunning() ? "true" : "false") + R"(
        })";
        writeUIData(ui_data);

        this_thread::sleep_for(chrono::seconds(5));
    }
}

// Enhanced main body with UI integration
void main_body(string filename, string enter_data, char name[25], char target, char password[25], char set_password[25], int login, int option, int control_panel, int user_account, int clock_region, int date_time, int network_internet, int file_manegment, int this_pc, int about_pc)
{
    // Start system monitoring thread
    thread monitor_thread(monitorSystem);

    while (1)
    {
        cout << "============================================" << endl;
        cout << "1.Control Pannel\n2.File Manegment\n3.About PC\n4.Shutdown\n5.Launch UI\n";
        cout << "============================================\n>>";
        cin >> option;

        if (option == 1)
        {
            while (1)
            {
                cout << "\t\t\t\tControl Panel\n";
                cout << "1.User Account\n2.Clock and Region\n3.Netword and Internet\n4.System and Security\n5.Exit\n";
                cout << ">>";
                cin >> control_panel;
                if (control_panel == 1)
                {
                    while (1)
                    {
                        cout << "\t\t\t\tControl Panel\\User Account\n";
                        cout << "1.Change Name\n2.Change Password\n3.Exit\n>>";
                        cin >> user_account;
                        if (user_account == 1)
                        {
                            cout << "Change Name\n";
                            cout << "Name:";
                            cin >> setw(24) >> name;
                            current_username = string(name);
                            cout << "Name updated to: " << current_username << "\n";
                        }
                        else if (user_account == 2)
                        {
                            cout << "Change Password\n";
                            cout << "Password:";
                            cin >> setw(24) >> set_password;
                            current_password = string(set_password);
                            cout << "Password updated!\n";
                        }
                        else if (user_account == 3)
                        {
                            cout << "Exit\n";
                            break;
                        }
                    }
                }
                else if (control_panel == 2)
                {
                    while (1)
                    {
                        cout << "\t\t\t\tControl Panel\\Clock and Region\n";
                        cout << "1.Date & Time\n2.Region\n3.Exit\n>>";
                        cin >> clock_region;
                        if (clock_region == 1)
                        {
                            cout << "Date & Time\n";
                            cout << "1.Date\n2.Time\n>>";
                            cin >> date_time;
                            if (date_time == 1)
                            {
                                time_t now = time(0);
                                tm *ltm = localtime(&now);
                                cout << "Date :" << ltm->tm_mday << "/" << (1 + ltm->tm_mon) << "/" << (1900 + ltm->tm_year) << endl;
                            }
                            else if (date_time == 2)
                            {
                                time_t now = time(0);
                                tm *ltm = localtime(&now);
                                cout << "time:" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << endl;
                            }
                        }
                        else if (clock_region == 2)
                        {
                            cout << "\t\t\t\tControl Panel\\Region\n";
                            time_t now = time(0);
                            tm *ltm = localtime(&now);
                            cout << "Date :" << ltm->tm_mday << "/" << (1 + ltm->tm_mon) << "/" << (1900 + ltm->tm_year) << endl;
                            cout << "time:" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << endl;
                        }
                        else if (clock_region == 3)
                        {
                            cout << "Exit\n";
                            break;
                        }
                    }
                }
                else if (control_panel == 3)
                {
                    cout << "\t\t\t\tControl Panel\\Network and Internet\n";
                    cout << "1.Network and Sharing Center\n2.Internet Option\n>>";
                    cin >> network_internet;
                    if (network_internet == 1)
                    {
                        cout << "Network and Sharing Center\n";
                        cout << "1.Setup a new connection or Network\n2.Troubleshoot problem\n";
                    }
                    else if (network_internet == 2)
                    {
                        cout << "Internet Option\n";
                    }
                }
                else if (control_panel == 4)
                {
                    cout << "\t\t\t\tControl Panel\\System And Security\n";
                }
                else if (control_panel == 5)
                {
                    cout << "Exit" << endl;
                    break;
                }
            }
        }
        else if (option == 2)
        {
            while (1)
            {
                cout << "File Manegment\n";
                cout << "1.This PC\n2.Document\n3.Desktop\n4.Downloads\n5.Picture\n6.Music\n7.Video\n8.Exit\n";
                cout << ">>";
                cin >> file_manegment;
                if (file_manegment == 1)
                {
                    while (1)
                    {
                        cout << "\t\t\t\tFile Manegment\\This PC\n";
                        cout << "Device and Drives\n";
                        cout << "1.OS(C)\t2.Local Disk(D)\t3.DVD RW Drive(E)\t4.Exit\n>>";
                        cin >> this_pc;
                        if (this_pc == 1)
                        {
                            storage(target);
                        }
                        else if (this_pc == 2)
                        {
                            storage(target);
                        }
                        else if (this_pc == 3)
                        {
                            storage(target);
                        }
                        else if (this_pc == 4)
                        {
                            cout << "Exit\n";
                            break;
                        }
                    }
                }
                else if (file_manegment == 2)
                {
                    cout << "\t\t\t\tFile Manegment\\Documenet\n";
                    cout << "Create File\n";
                    makefile(filename, enter_data);
                }
                else if (file_manegment == 3)
                {
                    cout << "\t\t\t\tFile Manegment\\Desktop\n";
                    onAppButton();
                }
                else if (file_manegment == 4)
                {
                    cout << "\t\t\t\tFile Manegment\\Dowloads\n";
                    cout << "Choram\nVS Code\nmingw-w64-v11.0.0\ngit2.52.0.64\nwhatsapp\n";
                }
                else if (file_manegment == 5)
                {
                    cout << "\t\t\t\tFile Manegment\\Picture\n";
                }
                else if (file_manegment == 6)
                {
                    cout << "\t\t\t\tFile Manegment\\Music\n";
                }
                else if (file_manegment == 7)
                {
                    cout << "\t\t\t\tFile Manegment\\Video\n";
                }
                else if (file_manegment == 8)
                {
                    cout << "Exit" << endl;
                    break;
                }
            }
        }
        else if (option == 3)
        {
            while (1)
            {
                cout << "\t\t\t\tAbout PC\n";
                cout << "1.Ram Memory\n2.Exit\n>>";
                cin >> about_pc;
                if (about_pc == 1)
                {
                    ram();
                }
                else if (about_pc == 2)
                {
                    cout << "Exit\n";
                    break;
                }
            }
        }
        else if (option == 4)
        {
            cout << "Shutdown\n";
            ui_launched = false;
            break;
        }
        else if (option == 5)
        {
            launchUI();
        }
    }

    // Wait for monitor thread to finish
    if (monitor_thread.joinable())
    {
        monitor_thread.join();
    }
}

int main()
{
    string filename = "", enter_data = "";
    int login = 0;
    char name[25] = "User";
    char target = 'C';
    char password[25] = {0}, set_password[25] = "1234";
    int option = 0;
    int control_panel = 0, user_account = 0, clock_region = 0, date_time = 0, network_internet = 0;
    int file_manegment = 0, this_pc = 0;
    int about_pc = 0;

    cout << "\t\t\t\t\t\t===== User Login =====\n";
    cout << "1.Login as Admin\n2.Login as Guest\n>>";
    cin >> login;

    if (login == 1)
    {
        while (1)
        {
            cout << "\t\t\t\t\t\t===== Admin =====\n";
            cout << name;
            cout << "\nPassword:";
            cin >> setw(24) >> password;
            if (strcmp(set_password, password) == 0)
            {
                cout << "\t\t\t\t\t\tWindow Open\n";
                break;
            }
            else
            {
                cout << "Invalid Password!\n";
            }
        }
        main_body(filename, enter_data, name, target, password, set_password, login, option, control_panel, user_account, clock_region, date_time, network_internet, file_manegment, this_pc, about_pc);
    }
    else if (login == 2)
    {
        cout << "\t\t\t\t\t\t===== Guest =====\n";
        main_body(filename, enter_data, name, target, password, set_password, login, option, control_panel, user_account, clock_region, date_time, network_internet, file_manegment, this_pc, about_pc);
    }

    return 0;
}
